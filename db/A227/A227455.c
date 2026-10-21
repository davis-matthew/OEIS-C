/*
 * A227455  Sequence defined recursively: 1 is in the sequence, and k > 1 is
 *          in the sequence iff for some prime divisor p of k, p-1 is not in
 *          the sequence.
 * https://oeis.org/A227455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For k > 1, k is in the sequence iff there exists a
 *            prime divisor p of k such that (p-1) is NOT in the sequence
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* pre‑defined integer types
   seq_t   – unsigned integer of the chosen size (uint8_t … uint64_t or big int)
   idx_t   – unsigned 64‑bit index (uint64_t) */
  
/* maximum number to sieve – large enough for n ≤ 10000 */
enum { MAX_LIMIT     = 2000000U,   /* change if larger values are needed   */
       MAX_PRIME_CNT = 170000U };  /* number of primes below MAX_LIMIT     */

static unsigned char in_seq   [MAX_LIMIT + 1]; /* membership flag  */
static unsigned char composite[MAX_LIMIT + 1]; /* sieve flag       */
static uint64_t     primes    [MAX_PRIME_CNT];/* list of primes   */
static idx_t        prime_cnt;   /* how many primes found            */
static idx_t        last_processed;/* last integer examined             */
static size_t       seq_cap;      /* capacity of seq_arr              */
static seq_t       *seq_arr;      /* array of sequence terms           */
static size_t       seq_len;      /* how many sequence terms stored    */
static bool         initialized;  /* one‑time init flag                */

/*------------------------------------------------------------------*/
seq_t a(idx_t n)
/*------------------------------------------------------------------*/
{
    /* ---------- one‑time initialization ---------- */
    if (!initialized) {
        /* 1 is the first term */
        in_seq[1] = 1;

        /* sieve to collect all primes ≤ MAX_LIMIT */
        prime_cnt = 0;
        for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
            if (!composite[i]) {
                if (prime_cnt < MAX_PRIME_CNT)
                    primes[prime_cnt++] = i;
                if (i * i <= MAX_LIMIT) {
                    for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                        composite[j] = 1;
                }
            }
        }

        /* allocate storage for sequence terms */
        seq_cap = 128;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();            /* allocation failure */
        seq_arr[0] = 1;                   /* a(1) = 1          */
        seq_len   = 1;
        last_processed = 1;

        initialized = true;
    }

    /* ---------- answer request for small n ---------- */
    if (n <= seq_len)
        return seq_arr[n - 1];

    /* ---------- extend sequence until we have n terms ---------- */
    while (seq_len < (size_t)n) {
        ++last_processed;
        if (last_processed > MAX_LIMIT)
            abort();  /* value out of bounds – should not happen for n ≤ 10000 */

        bool in = false;
        idx_t temp = last_processed;

        /* factor using precomputed primes */
        for (idx_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= temp; ++pi) {
            idx_t p = primes[pi];
            if (temp % p == 0) {
                if (!in_seq[p - 1]) {   /* (p-1) not in sequence → k in sequence */
                    in = true;
                    break;
                }
                while (temp % p == 0)
                    temp /= p;
            }
        }

        /* remaining prime factor, if any */
        if (!in && temp > 1) {
            if (!in_seq[temp - 1])
                in = true;
        }

        in_seq[last_processed] = in ? 1 : 0;

        if (in) {   /* store the new term */
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) abort();
            }
            seq_arr[seq_len++] = (seq_t)last_processed;
        }
    }

    return seq_arr[n - 1];
}
