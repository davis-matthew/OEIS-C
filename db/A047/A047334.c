/*
 * A047334  Duplicate of A032775.
 * https://oeis.org/A047334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers   a(n) are the natural numbers that are not congruent to 4 modulo 7.
   The sequence starts a(1)=0.  For each n≥1:
        b  = ⌊(n‑1)/6⌋          // block number
        r  = (n‑1) mod 6        // position inside the block (0…5)
        offset = (r < 4) ? r : r+1   // skip the 7b+4 term inside the block
   Then a(n) = 7*b + offset. */
seq_t a(idx_t n)
{
    /* assuming n ≥ 1 */
    idx_t b   = (n - 1) / 6;          /* number of completed 6‑term blocks */
    idx_t r   = (n - 1) % 6;          /* position within the block (0…5) */
    seq_t offset = (r < 4) ? (seq_t)r : (seq_t)(r + 1);
    return (seq_t)( (seq_t)7 * (seq_t)b + offset );
}
