/*
 * A053562  Number of ternary Lyndon words of length n with trace 1 and
 *          subtrace 0 over GF(3). Same as the number of ternary Lyndon words
 *          of length n with trace 2 and subtrace 0 over GF(3).
 * https://oeis.org/A053562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * [ Σ_{d|n, d≡1(3)} μ(d) M(n/d,0,1) * + Σ_{d|n,
 *            d≡2(3)} μ(d) M(n/d,0,2) ] * where M(n,t,s) = Σ (n!)/(i!j!k!)
 *            over i+j+k=n, j≡t (mod 3), k≡s (mod 3)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*-------------------------------------------*/
/*  Helper: Möbius function μ(n)             */
/*-------------------------------------------*/
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    idx_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            int c = 0;
            while (n % d == 0) {
                n /= d;
                c++;
                if (c > 1) return 0;          /* square factor   */
            }
            cnt++;
        }
        d++;
    }
    if (n > 1) cnt++;
    return (cnt % 2 == 0) ? 1 : -1;
}

/*-------------------------------------------*/
/*  Dynamic cache of M(n,t,s) values         */
/*  counts[ n*9 + a*3 + b ]                   */
/*-------------------------------------------*/
static seq_t *counts = NULL;      /* flattened array   */
static idx_t  max_cached_n = 0;   /* largest n cached  */

static void ensure_counts(idx_t n)
{
    if (n <= max_cached_n) return;

    size_t old_len = (max_cached_n + 1) * 9;
    size_t new_len = (n + 1) * 9;
    seq_t *new_counts = (seq_t *)realloc(counts, new_len * sizeof(seq_t));
    if (!new_counts) exit(1);          /* allocation failed */
    counts = new_counts;

    /* zero new entries */
    if (new_len > old_len)
        memset((void *)(counts + old_len), 0,
               (new_len - old_len) * sizeof(seq_t));

    /* base case: M(0,0,0)=1 */
    if (max_cached_n == 0)
        counts[0] = 1;

    /* incremental DP from old max to new n */
    for (idx_t k = max_cached_n; k < n; ++k) {
        for (int a = 0; a < 3; ++a) {
            for (int b = 0; b < 3; ++b) {
                seq_t val = counts[k * 9 + a * 3 + b];
                if (!val) continue;
                /* letter 0 keeps trace & subtrace  */
                counts[(k + 1) * 9 + a * 3 + b] += val;
                /* letter 1 increments trace   */
                counts[(k + 1) * 9 + ((a + 1) % 3) * 3 + b] += val;
                /* letter 2 increments subtrace */
                counts[(k + 1) * 9 + a * 3 + ((b + 1) % 3)] += val;
            }
        }
    }
    max_cached_n = n;
}

/*-------------------------------------------*/
/*  Main sequence function                   */
/*-------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; value for n=0 is unused */
    ensure_counts(n);

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* iterate over all divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        /* process divisor d */
        if (d % 3 == 1 || d % 3 == 2) {
            int mu = mobius(d);
            if (mu) {
                seq_t val = counts[(n / d) * 9 + 0 * 3 + (d % 3 == 1 ? 1 : 2)];
                if (mu > 0) sum_pos += val;
                else sum_neg += val;
            }
        }

        idx_t d2 = n / d;
        if (d2 == d) continue;   /* already handled */
        if (d2 % 3 == 1 || d2 % 3 == 2) {
            int mu = mobius(d2);
            if (mu) {
                seq_t val = counts[(n / d2) * 9 + 0 * 3 + (d2 % 3 == 1 ? 1 : 2)];
                if (mu > 0) sum_pos += val;
                else sum_neg += val;
            }
        }
    }

    /* result must be non‑negative */
    seq_t total = (sum_pos >= sum_neg) ? (sum_pos - sum_neg) : 0;
    return total / (seq_t)n;
}
