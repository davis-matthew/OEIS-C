/*
 * A033989  Write 0,1,2,... in a clockwise spiral on a square lattice, writing
 *          each digit at a separate lattice point, starting with 0 at the
 *          origin and 1 at x=0, y=-1; sequence gives the numbers on the
 *          negative x-axis.
 * https://oeis.org/A033989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = digitAt(4*n*n - n) where digitAt(k) is the k‑th digit
 *            (0‑based) of the infinite concatenation of the decimal
 *            representations of the natural numbers 0, 1, 2, 3, …
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t pow10[11] = {
    1ULL,          /* 10^0 */
    10ULL,         /* 10^1 */
    100ULL,        /* 10^2 */
    1000ULL,       /* 10^3 */
    10000ULL,      /* 10^4 */
    100000ULL,     /* 10^5 */
    1000000ULL,    /* 10^6 */
    10000000ULL,   /* 10^7 */
    100000000ULL,  /* 10^8 */
    1000000000ULL, /* 10^9 */
    10000000000ULL /* 10^10 (used only when d ≤ 10) */
};

static seq_t digit_at(idx_t k)
{
    /* 1‑digit numbers 0…9 give the first 10 digits */
    if (k < 10)
        return (seq_t)(k % 10);

    idx_t idx = k - 10;              /* remaining offset after the 1‑digit block */
    idx_t d   = 2;                   /* current digit length */

    /* iterate over blocks of numbers of equal digit length */
    while (1) {
        idx_t block_digits = 9ULL * pow10[d - 1] * d;  /* 9*10^(d-1) numbers */
        if (idx < block_digits)
            break;
        idx -= block_digits;
        d++;
    }

    /* locate the exact number and the specific digit within that number */
    idx_t number_index = idx / d;                    /* 0‑based offset in this block */
    idx_t number       = pow10[d - 1] + number_index;/* the actual number */
    idx_t digit_index  = idx % d;                    /* 0‑based index from the most significant digit */

    /* extract the required digit */
    idx_t div = pow10[d - 1 - digit_index];
    seq_t digit = (seq_t)((number / div) % 10);
    return digit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* step index for coordinate (-n,0) along the spiral */
    idx_t k = 4ULL * n * n - n;
    return digit_at(k);
}
