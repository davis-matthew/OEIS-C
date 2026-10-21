/*
 * A208646  Calendar Problem #27, April 2012 Mathematics Teacher.
 * https://oeis.org/A208646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A208646: 
   The terms are the 4‑digit numbers that are permutations of 1,6,8,9
   (no repeated digits), sorted in descending numeric order.
   Each such number is followed by the negative of its 180° rotational
   partner (obtained by reversing the digits and swapping 6↔9, 1,8 remain unchanged).
   The first six such numbers are 9861, 9681, 8961, 8691, 6981, 6891. 
   Their partners (in the range 1000‑1999) are 1986, 1896, 1968, 1698, 1869, 1689.
*/

seq_t a(idx_t n) {
    static const seq_t seq[] = {
        9861, -1986,
        9681, -1896,
        8961, -1968,
        8691, -1698,
        6981, -1869,
        6891, -1689
    };
    if (n == 0) return 0;
    if (n <= (sizeof(seq) / sizeof(seq[0]))) return seq[n - 1];
    return 0;
}
