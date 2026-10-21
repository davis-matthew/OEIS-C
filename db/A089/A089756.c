/*
 * A089756  a(1)=1 and a(i+1)=a(i)+9 if a(i)<=35, and a(i+1)=a(i)-35 if
 *          a(i)>35.
 * https://oeis.org/A089756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (9*(n-1) mod 44) + 1 (equivalent to the stated
 *            recurrence)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the position in the 44‑term cycle. */
    idx_t r = (n - 1) % 44;               /* r ∈ [0,43] */

    /* Closed‑form evaluation; result is in [1,44]. */
    seq_t val = ((seq_t)9 * (seq_t)r) % (seq_t)44 + 1;

    return val;
}
