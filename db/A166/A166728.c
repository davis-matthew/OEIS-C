/*
 * A166728  Positive integers with English names ending in "x".
 * https://oeis.org/A166728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = 10 * (10 * floor((n-1)/9) + ( (n-1)%9 == 0 ? 0 :
 *            ((n-1)%9)+1 )) + 6
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all integers ending in 6, except those whose last two digits are 16. */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    idx_t nm1 = n - 1;                 /* (n-1) – always safe because n ≥ 1 */
    idx_t q   = nm1 / 9;               /* number of complete 9‑term blocks before n */
    idx_t r   = nm1 % 9;               /* position inside the current block (0…8) */
    seq_t base_k = (seq_t)(10ULL * q); /* base k = 10*q, cast to seq_t for further use */

    /* Adjust k according to the block:
       positions 0→k+=0  (0), 1→k+=2  (2), 2→k+=3  (3), …, 8→k+=9  (9)   */
    seq_t k = base_k + (r == 0 ? 0 : r + 1);

    /* Finally compute a(n) = 10*k + 6 */
    return (seq_t)10 * k + (seq_t)6;
}
