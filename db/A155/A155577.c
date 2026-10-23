/*
 * A155577  Intersection of A154777 and A154778: N = a^2 + 2b^2 = c^2 + 5d^2
 *          for some positive integers a,b,c,d.
 * https://oeis.org/A155577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Intersection of A154777 and A154778 a(n) = the nth positive
 *            integer that can be represented as a^2 + 2b^2 and as c^2 + 5d^2
 *            with a,b,c,d ≥ 1
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* helper: floor sqrt for 64‑bit unsigned */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static storage for terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static idx_t current_limit = 0;

    if (n == 0) return (seq_t)0;            /* undefined, guard */

    /* need more terms? */
    if (terms_len < n) {
        idx_t new_limit = current_limit ? current_limit : 1024;
        /* grow until we have enough terms */
        while (terms_len < n) {
            new_limit <<= 1;                /* double the search limit */
            /* allocate marker arrays */
            uint8_t *rep1 = (uint8_t*)calloc((size_t)new_limit + 1, sizeof(uint8_t));
            uint8_t *rep2 = (uint8_t*)calloc((size_t)new_limit + 1, sizeof(uint8_t));
            if (!rep1 || !rep2) {
                /* allocation failed – return 0 */
                if (rep1) free(rep1);
                if (rep2) free(rep2);
                return (seq_t)0;
            }

            /* numbers of the form a^2 + 2b^2   (a,b ≥ 1) */
            for (idx_t a_val = 1; a_val * a_val <= new_limit; ++a_val) {
                uint64_t a2 = (uint64_t)a_val * a_val;
                uint64_t max_b_sq = new_limit - a2;
                if (max_b_sq < 2) continue;
                uint64_t max_b = isqrt_u64(max_b_sq / 2);
                for (idx_t b_val = 1; b_val <= max_b; ++b_val) {
                    uint64_t val = a2 + 2ULL * (uint64_t)b_val * (uint64_t)b_val;
                    rep1[(size_t)val] = 1;
                }
            }

            /* numbers of the form c^2 + 5d^2   (c,d ≥ 1) */
            for (idx_t c_val = 1; c_val * c_val <= new_limit; ++c_val) {
                uint64_t c2 = (uint64_t)c_val * c_val;
                uint64_t max_d_sq = new_limit - c2;
                if (max_d_sq < 5) continue;
                uint64_t max_d = isqrt_u64(max_d_sq / 5);
                for (idx_t d_val = 1; d_val <= max_d; ++d_val) {
                    uint64_t val = c2 + 5ULL * (uint64_t)d_val * (uint64_t)d_val;
                    rep2[(size_t)val] = 1;
                }
            }

            /* build term list from intersection */
            size_t new_len = 0;
            /* free previous array if present */
            if (terms) free(terms);
            /* estimate size: at most new_limit/2 terms (conservative) */
            terms = (seq_t*)malloc(sizeof(seq_t) * (new_limit / 2 + 1));
            if (!terms) {
                free(rep1);
                free(rep2);
                return (seq_t)0;
            }

            for (idx_t val = 1; val <= new_limit; ++val) {
                if (rep1[(size_t)val] && rep2[(size_t)val]) {
                    terms[new_len++] = (seq_t)val;
                }
            }
            terms_len = new_len;
            current_limit = new_limit;

            free(rep1);
            free(rep2);
        }
    }

    return terms[(size_t)n - 1];
}
