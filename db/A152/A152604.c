/*
 * A152604  a(1) = 1; thereafter a(n) is always the smallest integer > a(n-1)
 *          not leading to a contradiction, such that any four consecutive
 *          digits in the sequence sum up to a prime.
 * https://oeis.org/A152604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A152604: a(1) = 1;
   for n > 1, a(n) is the smallest integer > a(n‑1) such that
   when its decimal digits are appended to the running stream,
   every block of four consecutive digits sums to a prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ----------  globals for memoisation  ---------- */
static seq_t *term_cache       = NULL;   /* cached terms             */
static size_t term_cap         = 0;      /* capacity of term_cache   */
static idx_t  term_max         = 0;      /* number of terms cached   */

static unsigned char *digit_buf = NULL;  /* stream of digits (LSB first?) */
static size_t         digit_cap = 0;      /* allocated capacity        */
static idx_t          digit_len = 0;      /* number of digits in stream */

static bool prime_sum[37];
static bool prime_sum_init = false;

/* ----------  helper: grow the term cache  ---------- */
static void grow_terms(size_t needed)
{
    if (term_cap < needed) {
        size_t new_cap = term_cap ? term_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        term_cache = tmp;
        term_cap   = new_cap;
    }
}

/* ----------  helper: grow the digit buffer  ---------- */
static void grow_digits(size_t needed)
{
    if (digit_cap < needed) {
        size_t new_cap = digit_cap ? digit_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        unsigned char *tmp = (unsigned char*)realloc(digit_buf, new_cap * sizeof(unsigned char));
        if (!tmp) abort(); /* allocation failure */
        digit_buf = tmp;
        digit_cap = new_cap;
    }
}

/* ----------  initialise prime lookup  ---------- */
static void init_prime_sum(void)
{
    if (prime_sum_init) return;
    for (int i = 0; i < 37; ++i) {
        bool p = false;
        if (i >= 2) {
            p = true;
            for (int j = 2; j * j <= i; ++j)
                if (i % j == 0) { p = false; break; }
        }
        prime_sum[i] = p;
    }
    prime_sum_init = true;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1; behaviour irrelevant */

    init_prime_sum();

    /* Allocate / initialise caches on first call */
    if (!term_cache) {
        term_cache = (seq_t*)malloc(8 * sizeof(seq_t));
        if (!term_cache) abort();
        term_cap = 8;
    }
    if (!digit_buf) {
        digit_buf = (unsigned char*)malloc(16 * sizeof(unsigned char));
        if (!digit_buf) abort();
        digit_cap = 16;
    }

    /* Return cached value if available */
    if (n <= term_max) return term_cache[n-1];

    /* Seed first term if not yet seeded */
    if (term_max == 0) {
        term_cache[0] = (seq_t)1;   /* a(1) = 1 */
        term_max = 1;

        /* initialise digit stream with digits of 1 */
        digit_buf[0] = 1;
        digit_len = 1;
    }

    /* Compute terms up to n */
    for (idx_t idx = term_max; idx < n; ++idx) {
        seq_t prev = term_cache[idx-1];
        seq_t cand = prev + (seq_t)1;

        for (;;) {
            /* extract decimal digits of cand, most‑significant first */
            seq_t tmp = cand;
            unsigned int rev[50];
            int rev_cnt = 0;
            if (tmp == 0) {
                rev[rev_cnt++] = 0;
            } else {
                while (tmp > 0) {
                    rev[rev_cnt++] = (unsigned int)(tmp % (seq_t)10);
                    tmp = tmp / (seq_t)10;
                }
            }

            /* try to append these digits, backing up if the test fails */
            idx_t start_len = digit_len;
            bool ok = true;

            for (int k = rev_cnt-1; k >= 0; --k) {
                unsigned int d = rev[k];
                grow_digits(digit_len + 1);
                digit_buf[digit_len++] = (unsigned char)d;

                if (digit_len >= 4) {
                    int s = (int)digit_buf[digit_len-1]
                           + (int)digit_buf[digit_len-2]
                           + (int)digit_buf[digit_len-3]
                           + (int)digit_buf[digit_len-4];
                    if (!prime_sum[s]) {
                        ok = false;
                        break;
                    }
                }
            }

            if (ok) {
                /* candidate accepted */
                grow_terms(term_max + 1);
                term_cache[term_max++] = cand;
                break;  /* proceed to next term */
            } else {
                /* rollback digits and try next candidate */
                digit_len = start_len;
                ++cand;
            }
        }
    }

    return term_cache[n-1];
}
