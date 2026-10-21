/*
 * A094328  Iterate the map in A006369 starting at 4.
 * https://oeis.org/A094328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094328:  Iterate the map in A006369 starting at 4.  
   The sequence is periodic with period length 5:  
   a(n) = 4, 5, 7, 9, 6, 4, 5, 7, 9, 6, ...   */
seq_t a(idx_t n)
{
    /* Periodic pattern starting from index 1 */
    static const seq_t period[5] = {4, 5, 7, 9, 6};
    /* (n-1) % 5 gives the correct 0‑based index into the period */
    return period[(n - 1) % 5];
}
