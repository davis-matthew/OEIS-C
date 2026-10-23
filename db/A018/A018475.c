/*
 * A018475  Divisors of 475.
 * https://oeis.org/A018475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018475: a(n) is the n-th divisor of 475 in increasing order. */
/* Divisors are generated from the prime factorisation 5^2 * 19. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* powers of 5 (exponents 0..2) */
    seq_t pow5[3];
    pow5[0] = 1;
    pow5[1] = (seq_t)5;
    pow5[2] = pow5[1] * pow5[1];   /* 25 */

    /* powers of 19 (exponents 0..1) */
    seq_t pow19[2];
    pow19[0] = 1;
    pow19[1] = (seq_t)19;

    /* produce all 6 divisors */
    seq_t divs[6];
    idx_t idx = 0;
    for (idx_t a = 0; a < 3; ++a)
        for (idx_t b = 0; b < 2; ++b)
            divs[idx++] = pow5[a] * pow19[b];

    /* sort the 6 divisors (bubble sort is fine for such a small array) */
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5 - i; ++j)
            if (divs[j] > divs[j + 1]) {
                seq_t t = divs[j];
                divs[j] = divs[j + 1];
                divs[j + 1] = t;
            }

    if (n <= 6) return divs[n - 1];
    return 0;
}
