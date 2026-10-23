/*
 * A178858  Divisors of 5040.
 * https://oeis.org/A178858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 5040 in ascending order, a(1)=1, a(60)=5040. */
#include <stdbool.h>
#include <stdlib.h>  /* qsort */

static bool init = false;
static seq_t divisors[60];

static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t*)a;
    seq_t y = *(const seq_t*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Initialise the divisor list once. */
static void init_divisors(void)
{
    if (init) return;

    /* Pre‑compute powers of the primes 2,3,5,7. */
    seq_t pow2[5], pow3[3], pow5[2], pow7[2];
    pow2[0] = pow3[0] = pow5[0] = pow7[0] = (seq_t)1;
    for (int i = 1; i < 5; i++) pow2[i] = pow2[i-1] * (seq_t)2;
    for (int i = 1; i < 3; i++) pow3[i] = pow3[i-1] * (seq_t)3;
    for (int i = 1; i < 2; i++) pow5[i] = pow5[i-1] * (seq_t)5;
    for (int i = 1; i < 2; i++) pow7[i] = pow7[i-1] * (seq_t)7;

    size_t idx = 0;
    for (int i = 0; i <= 4; i++)          /* exponent of 2 */
    for (int j = 0; j <= 2; j++)          /* exponent of 3 */
    for (int k = 0; k <= 1; k++)          /* exponent of 5 */
    for (int l = 0; l <= 1; l++)          /* exponent of 7 */
    {
        divisors[idx++] = pow2[i] * pow3[j] * pow5[k] * pow7[l];
    }

    qsort(divisors, 60, sizeof(seq_t), cmp_seq);
    init = true;
}

seq_t a(idx_t n)
{
    init_divisors();

    if (n < 1 || n > 60) return (seq_t)0;   /* undefined beyond 60 */
    return divisors[(size_t)(n-1)];
}
