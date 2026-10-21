/*
 * A204272  a(n) = sigma_2(n)*Pell(n), where sigma_2(n) = A001157(n), the sum
 *          of squares of divisors of n.
 * https://oeis.org/A204272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma_2(n) * Pell(n) where sigma_2(n) = sum of squares of
 *            divisors of n, Pell(0)=0, Pell(1)=1,
 *            Pell(n)=2*Pell(n-1)+Pell(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pell_tbl = NULL;
static idx_t pell_max  = 0;

/* Grow Pell table to include value n */
static void ensure_pell(idx_t n)
{
    if (n <= pell_max) return;

    size_t new_size = (size_t)n + 1;
    if (pell_tbl == NULL) {
        pell_tbl = (seq_t*)malloc(new_size * sizeof(seq_t));
    } else {
        pell_tbl = (seq_t*)realloc(pell_tbl, new_size * sizeof(seq_t));
    }
    if (pell_tbl == NULL) exit(1);  /* allocation failed */

    /* Initialise base cases if table is new */
    if (pell_max == 0) {
        pell_tbl[0] = (seq_t)0;
        if (new_size > 1) pell_tbl[1] = (seq_t)1;
        pell_max = 1;
    }

    /* Compute missing Pell numbers */
    for (idx_t i = pell_max + 1; i <= n; ++i) {
        pell_tbl[i] = (seq_t)2 * pell_tbl[i-1] + pell_tbl[i-2];
    }
    pell_max = n;
}

/* Sum of squares of divisors of n */
static seq_t sigma2(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j   = n / i;
            seq_t ii  = (seq_t)i * i;
            if (j == i) {
                sum += ii;
            } else {
                sum += ii + (seq_t)j * j;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sigma_val = sigma2(n);
    ensure_pell(n);
    seq_t p = pell_tbl[n];
    return sigma_val * p;
}
