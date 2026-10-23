/*
 * A166731  Positive integers with English names ending in "d".
 * https://oeis.org/A166731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = 100 * ( ((n-1)/9999)*10000 + ((n-1)%9999 + 1) )
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positive integers with English names ending in "d" */
/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* Guard against zero index (undefined in OEIS). Return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* block: which group of 9,999 qualifying multiples of 100 */
    idx_t block  = (n - 1) / 9999;
    /* offset inside the block (0‑based) */
    idx_t offset = (n - 1) % 9999;

    /* t is the k‑th multiple of 100 that is not a multiple of 1,000,000 */
    seq_t t = (seq_t)block * 10000 + (seq_t)(offset + 1);

    /* Each qualifying number is 100 * t. */
    return t * 100;
}
