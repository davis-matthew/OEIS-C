/*
 * A217253  Number of minimal length formulas representing n only using
 *          addition, multiplication, exponentiation and the constant 1.
 * https://oeis.org/A217253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217253: a(n) = number of minimal‑length postfix expressions over 
 * {1, +, *, ^} that evaluate to n.  The function computes the sequence
 * by dynamic programming, storing the minimal length and the number of
 * expressions that attain that length. */
#include <stdlib.h>

static idx_t *len_arr = NULL;     /* minimal length for each n */
static seq_t *cnt_arr = NULL;     /* number of minimal expressions for n */
static idx_t  max_computed = 0;   /* largest n that has been computed */

/* ensure that tables are computed for all indices up to n (inclusive) */
static void ensure_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    size_t new_cap = (size_t)n + 1U;

    /* enlarge tables */
    if (len_arr == NULL) {
        len_arr = (idx_t *)malloc(new_cap * sizeof(idx_t));
        cnt_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        len_arr = (idx_t *)realloc(len_arr, new_cap * sizeof(idx_t));
        cnt_arr = (seq_t *)realloc(cnt_arr, new_cap * sizeof(seq_t));
    }
    if (len_arr == NULL || cnt_arr == NULL)
        exit(1);  /* allocation failed */

    /* compute values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {              /* base case */
            len_arr[1] = 1;
            cnt_arr[1] = (seq_t)1;
            continue;
        }

        idx_t best_len = (idx_t)-1;          /* large value */
        seq_t best_cnt = (seq_t)0;

        /* 1. addition splits */
        for (idx_t x = 1; x <= i - 1; ++x) {
            idx_t y = i - x;
            idx_t cand_len = len_arr[x] + len_arr[y] + 1U;
            seq_t ways = cnt_arr[x] * cnt_arr[y];

            if (cand_len < best_len) {
                best_len = cand_len;
                best_cnt = ways;
            } else if (cand_len == best_len) {
                best_cnt += ways;
            }
        }

        /* 2. multiplication divisors (both factors > 1) */
        for (idx_t d = 2; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t a = d;
                idx_t b = i / d;
                idx_t cand_len = len_arr[a] + len_arr[b] + 1U;
                seq_t ways = cnt_arr[a] * cnt_arr[b];
                if (a != b)
                    ways *= (seq_t)2;   /* two orders */

                if (cand_len < best_len) {
                    best_len = cand_len;
                    best_cnt = ways;
                } else if (cand_len == best_len) {
                    best_cnt += ways;
                }
            }
        }

        /* 3. exponentiation pairs (base >= 2, exponent >= 2) */
        for (idx_t base = 2; base * base <= i; ++base) {
            idx_t pow = base * base;
            idx_t exp = 2;
            while (pow <= i) {
                if (pow == i) {
                    idx_t a = base;
                    idx_t b = exp;
                    idx_t cand_len = len_arr[a] + len_arr[b] + 1U;
                    seq_t ways = cnt_arr[a] * cnt_arr[b];

                    if (cand_len < best_len) {
                        best_len = cand_len;
                        best_cnt = ways;
                    } else if (cand_len == best_len) {
                        best_cnt += ways;
                    }
                }
                ++exp;
                pow *= base;
            }
        }

        len_arr[i] = best_len;
        cnt_arr[i] = best_cnt;
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* not defined for n = 0, but return 0 safely */
    ensure_up_to(n);
    return cnt_arr[n];
}
