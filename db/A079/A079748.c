/*
 * A079748  Largest k such that the greatest prime factors from n to n+k are
 *          monotonically increasing.
 * https://oeis.org/A079748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A079748:
   a(n) is the largest k such that the greatest prime factors of
   n, n+1, … , n+k are strictly increasing. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global state: largest prime factor array (uint32_t) up to max_limit */
static size_t max_limit = 0;          /* current maximum index pre‑computed */
static uint32_t *lpf_arr = NULL;      /* lpf_arr[i] = largest prime factor of i */

/* ------------------------------------------------------------ */
/*   Ensure that lpf_arr is computed for all indices up to up_to  */
static void ensure_limit(idx_t up_to)
{
    if (up_to <= max_limit) return;          /* already done */

    /* Grow the array (double each time) until it covers up_to */
    size_t new_limit = (max_limit == 0) ? 2 : max_limit * 2;
    while (new_limit < (size_t)up_to) new_limit *= 2;

    /* Allocate a fresh array and compute all largest prime factors
       by a simple sieve.  The table is rebuilt entirely for clarity,
       which is inexpensive for the expected size (≈10 000). */
    uint32_t *new_lpf = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_lpf) abort();                    /* OOM */

    for (idx_t i = 2; i <= (idx_t)new_limit; ++i) {
        if (new_lpf[i] == 0) {                /* i is prime */
            for (idx_t j = i; j <= (idx_t)new_limit; j += i)
                new_lpf[j] = (uint32_t)i;    /* record i as the current (largest) prime factor */
        }
    }

    /* Replace the old table */
    if (lpf_arr) free(lpf_arr);
    lpf_arr   = new_lpf;
    max_limit = new_limit;
}

/* ------------------------------------------------------------ */
/*  OEIS sequence A079748 */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 we return 0. */
    if (n == 0) return (seq_t)0;

    /* Make sure we have lpf data for n and n+1 at least. */
    ensure_limit(n + 1);

    seq_t k = (seq_t)0;          /* length of the monotone block */
    idx_t cur = n;               /* current position */

    for (;; ) {
        /* If we run out of pre‑computed data while extending the block,
           enlarge the table on demand. */
        if (cur + 1 > max_limit) ensure_limit(cur + 1);

        /* Compare largest prime factors.  They are stored as uint32_t,
           but the array index is cast to size_t for C++. */
        if (lpf_arr[(size_t)cur] < lpf_arr[(size_t)(cur + 1)]) {
            ++k;                  /* the block can be extended */
            ++cur;
        } else {
            break;                /* first violation of monotonicity */
        }
    }
    return k;
}
