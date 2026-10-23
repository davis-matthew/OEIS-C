/*
 * A125844  a(n) = the smallest positive integer which does not occur earlier
 *          in the sequence and which is coprime to the n-th composite
 *          integer.
 * https://oeis.org/A125844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125844: a(n) = the smallest unused positive integer that is
   coprime to the n-th composite integer.  The sequence is generated
   iteratively; the first term is a(1)=1.  No closed‑form formula is known. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_primes.h"

static seq_t *seq = NULL;          /* generated terms */
static size_t seq_len = 0;         /* number of terms stored */

static uint8_t *used = NULL;      /* flags for used numbers */
static size_t used_len = 0;        /* current allocated size of 'used' */

/* Composite numbers 4,6,8,9,10,…   (stored 0‑based) */
static idx_t *composites = NULL;
static size_t composites_len = 0;
static idx_t next_comp = 4;        /* next integer to test for compositeness */

/* Primes for primality testing */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;

/* initialise the prime table once */
static void init_primes(void)
{
    if (primes_initialized) return;
    const idx_t LIMIT = 200000;  /* covers composites up to ~120 000 */
    build_primes(LIMIT, &prime_list, &prime_cnt);
    primes_initialized = true;
}

/* primality test for uint64_t using the precomputed prime list */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ensure we have at least n composite numbers stored */
static void ensure_composites(idx_t n)
{
    init_primes();
    while (composites_len < (size_t)n) {
        if (!is_prime_candidate(next_comp)) {
            composites = (idx_t *)realloc(composites, sizeof(idx_t) * (composites_len + 1));
            composites[composites_len++] = next_comp;
        }
        ++next_comp;
    }
}

/* ensure the sequence has at least n terms computed */
static void ensure_sequence(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    seq = (seq_t *)realloc(seq, sizeof(seq_t) * n);
    for (idx_t i = seq_len; i < n; ++i) {
        /* the (i+1)-th composite is composites[i] */
        idx_t comp_val = composites[(size_t)i];
        seq_t comp = (seq_t)comp_val;

        seq_t candidate = 1;
        for (;;) {
            idx_t use_idx = (idx_t)candidate;
            if (use_idx >= used_len) {
                size_t new_len = use_idx * 2 + 1;
                used = (uint8_t *)realloc(used, new_len);
                memset(used + used_len, 0, new_len - used_len);
                used_len = new_len;
            }
            if (!used[use_idx] && gcd(candidate, comp) == (seq_t)1) {
                used[use_idx] = 1;
                seq[i] = candidate;
                break;
            }
            ++candidate;
        }
    }
    seq_len = n;
}

/* public function required by the runner */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined below offset */
    ensure_composites(n);
    ensure_sequence(n);
    return seq[n - 1];
}
