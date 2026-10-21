/*
 * A056762  Number of terms in A056761 (i.e., odd numbers from A056757)
 *          between 2^(n-1) and 2^n.
 * https://oeis.org/A056762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A056762:
 * a(n) = number of odd integers x such that 2^(n-1) < x <= 2^n and τ(x)^3 > x,
 * where τ(x) is the number of divisors of x.
 * The sequence is finite (all qualifying x ≤ 2^20), so a(n)=0 for n>20.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache of qualifying odd integers (sorted ascending) */
static seq_t *qual = NULL;
static size_t qual_cnt = 0;
static int qual_init = 0;

/* Build the array of qualifying odd integers 1 ≤ x ≤ 2^20 */
static void init_qual(void)
{
    if (qual_init) return;

    const idx_t LIMIT = (1ULL << 20);          /* 2^20 = 1,048,576 */
    /* Small‑prime‑factor sieve for 0..LIMIT */
    uint64_t *spf = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    if (!spf) return;                         /* memory allocation failed */

    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* Rough upper bound for the number of qualifying odd integers */
    size_t est = LIMIT / 2 + 1;
    seq_t *tmp = (seq_t *)malloc(est * sizeof(seq_t));
    if (!tmp) { free(spf); return; }

    size_t cnt = 0;
    for (idx_t x = 1; x <= LIMIT; x += 2) {               /* only odd x */
        idx_t n = x;
        seq_t tau = 1;
        while (n > 1) {
            uint64_t p = spf[n];
            uint64_t c = 0;
            while (n % p == 0) {
                n /= p;
                ++c;
            }
            tau = tau * (c + 1);
        }
        /* τ(x)^3 > x   (seq_t compared to idx_t is allowed) */
        if (tau * tau * tau > x) {
            tmp[cnt++] = x;
        }
    }

    /* Shrink array to exact size */
    qual = (seq_t *)realloc(tmp, cnt * sizeof(seq_t));
    qual_cnt = cnt;
    free(spf);
    qual_init = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!qual_init) init_qual();   /* ensure qualifiers are ready */

    /* For n > 20 there are no qualifying numbers in that interval. */
    if (n > 20) return 0;

    /* Compute interval (2^(n-1), 2^n] */
    idx_t lower = 1ULL << (n - 1); /* 2^(n-1) */
    idx_t upper = 1ULL << n;      /* 2^n */

    seq_t count = 0;
    for (size_t i = 0; i < qual_cnt; ++i) {
        seq_t x = qual[i];
        if (x > (seq_t)lower && x <= (seq_t)upper) {
            ++count;
        }
    }
    return count;
}
