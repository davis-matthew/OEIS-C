/*
 * A136800  Number of composites in prime gaps of size 3 or larger, in order
 *          of appearance.
 * https://oeis.org/A136800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A136799(n)-A136798(n)+1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   grow_primes_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136800: number of composites in prime gaps of size ≥3 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_primes_array.h"

/* ---- static storage for gaps, primes and sieve ----------- */
static seq_t   *gap_lengths  = NULL;   /* lengths of qualifying gaps (≥3)          */
static idx_t   gap_len_cnt  = 0;       /* number of gaps stored                   */
static size_t  gap_len_cap  = 0;       /* allocated capacity of the above array    */

static uint64_t *primes      = NULL;   /* list of primes found so far              */
static idx_t    primes_cnt   = 0;       /* number of primes stored                  */
static size_t   primes_cap   = 0;       /* allocated capacity of the primes array   */

static idx_t    processed_prime_index = 0;   /* index of last prime whose gap was examined */

static uint8_t  *is_comp     = NULL;   /* sieve flag array (0 – prime, 1 – composite)     */
static idx_t    max_checked = 0;       /* highest number currently sieved           */

/* ---------------------------------------------------------- */
/* Grow a seq_t array to hold at least `need` elements       */
static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t newcap = (*cap) ? (*cap) << 1 : 16;
        while (newcap < need) newcap <<= 1;
        *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* ---------------------------------------------------------- */

/* ---------------------------------------------------------- */
/* Extend the sieve to cover all numbers up to `new_limit`.   */
/* New qualifying gaps (length ≥3) are appended to the gap   */
/* array.  The list of primes is rebuilt from scratch.      */
static void extend_sieve(idx_t new_limit) {
    if (new_limit <= max_checked) return;

    /* (Re)allocate flag array */
    is_comp = (uint8_t*)realloc(is_comp, (size_t)(new_limit + 1));
    for (idx_t i = 0; i <= new_limit; ++i) is_comp[(size_t)i] = 0;

    /* Rebuild prime list */
    primes_cnt = 0;
    max_checked = new_limit;

    for (idx_t i = 2; i <= max_checked; ++i) {
        if (!is_comp[(size_t)i]) {
            grow_primes_array(&primes, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = (uint64_t)i;
            if (i > max_checked / i) continue;
            for (idx_t j = i * i; j <= max_checked; j += i)
                is_comp[(size_t)j] = 1;
        }
    }

    /* Record qualifying gaps after the already processed primes */
    for (idx_t idx = processed_prime_index + 1; idx < primes_cnt; ++idx) {
        uint64_t pprev = primes[idx - 1];
        uint64_t pcur  = primes[idx];
        uint64_t gap_len = pcur - pprev - 1;
        if (gap_len >= 3) {
            grow_seq_t_array(&gap_lengths, &gap_len_cap, gap_len_cnt + 1);
            gap_lengths[gap_len_cnt++] = (seq_t)gap_len;
        }
    }

    /* Update processed index to the last prime in the new list */
    processed_prime_index = primes_cnt - 1;
}

/* ---------------------------------------------------------- */
/* Return the n‑th term of sequence A136800 (1‑based).       */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety – the offset is 1 */

    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        extend_sieve(200);                    /* initial small sieve */
    }

    /* Fetch more gaps if needed */
    while ((idx_t)gap_len_cnt < n) {
        extend_sieve(max_checked * 2 + 100);  /* grow bound when needed */
    }

    return gap_lengths[n - 1];
}
