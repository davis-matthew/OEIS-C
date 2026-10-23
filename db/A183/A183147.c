/*
 * A183147  1 together with the numbers with property that at least two
 *          successive of their divisors are odd or even.
 * https://oeis.org/A183147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer whose sorted list of * divisors
 *            contains at least one pair of consecutive elements having * the
 *            same parity (both odd or both even). a(1)=1 is a special term
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(idx_t *)a;
    idx_t vb = *(idx_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* Return true if n has a pair of consecutive divisors with identical parity. */
static bool has_consecutive_same_parity(idx_t n)
{
    /* 1 is the first divisor; if n==1 we won't call this function. */
    idx_t sqrt_n = (idx_t)sqrt((double)n);
    /* Allocate enough space for all divisors: at most 2*sqrt_n+1. */
    size_t capacity = 2 * (size_t)sqrt_n + 1;
    idx_t *divs = (idx_t *)malloc(capacity * sizeof(idx_t));
    if (!divs) return false;   /* allocation failure, treat as false */
    size_t div_cnt = 0;

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            divs[div_cnt++] = i;
            idx_t j = n / i;
            if (j != i) divs[div_cnt++] = j;
        }
    }

    /* Sort divisors ascending. */
    qsort(divs, div_cnt, sizeof(idx_t), cmp_idx_t);

    /* Scan for consecutive same parity. */
    int prev_parity = -1;
    for (size_t k = 0; k < div_cnt; ++k) {
        int parity = divs[k] & 1;  /* 0 for even, 1 for odd */
        if (prev_parity != -1 && parity == prev_parity) {
            free(divs);
            return true;
        }
        prev_parity = parity;
    }

    free(divs);
    return false;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t count = 1;   /* counted the term 1 */
    idx_t num = 2;

    while (count < n) {
        if (has_consecutive_same_parity(num)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
        ++num;
    }
    return 0; /* unreachable */
}
