/*
 * A098728  Consider the sequence {b(n), n >= 1} of digits of the natural (or
 *          counting) numbers: 1 2 3 4 5 6 7 8 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1
 *          7 1 8 1 9 2 0... (A007376); a(n) = n - b(n).
 * https://oeis.org/A098728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n+1) - b(n+1), where b(k) is the k‑th digit in the
 *            concatenation 123456789101112
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* k = n+1 (1‑based position of the digit) */
    idx_t d = 1;                    /* number of digits of the numbers in this block */
    idx_t pow10 = 1;                /* 10^(d-1), first number in this block */
    unsigned __int128 pos = (unsigned __int128)(n + 1);  /* still in 1‑based */
    while (1) {
        unsigned __int128 numbers = 9 * (unsigned __int128)pow10;   /* count of numbers in block */
        unsigned __int128 block = numbers * d;                     /* digits contributed by this block */
        if (pos <= block)
            break;
        pos -= block;
        d++;
        pow10 *= 10;
    }
    /* Adjust pos to 1‑based index within the selected block */
    idx_t pos_in_block = (idx_t)pos;          /* 1 ≤ pos_in_block ≤ block digits */
    idx_t number_index = (pos_in_block - 1) / d;   /* zero‑based offset of the number */
    idx_t digit_index  = (pos_in_block - 1) % d;   /* zero‑based digit position in the number */
    idx_t number_start = pow10;                  /* first number with d digits */
    idx_t number = number_start + number_index;  /* the actual number */

    /* Compute the requested digit: most significant digit => digit_index = 0 */
    idx_t factor = 1;
    for (idx_t i = 0; i < d - digit_index - 1; ++i)
        factor *= 10;
    idx_t digit = (number / factor) % 10;        /* digit is in 0..9 */

    seq_t nseq = (seq_t)n;
    seq_t result = nseq + 1 - (seq_t)digit;      /* a(n) = (n+1) - digit */
    return result;
}
