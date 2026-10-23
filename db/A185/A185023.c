/*
 * A185023  Number of steps to reach the number 1 of the map n -> n/3 if x==0
 *          (mod 3), n -> (4n-1)/3 if x==1 (mod 3), n -> (4n+1)/3 if x==2 (mod
 *          3).
 * https://oeis.org/A185023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of steps to reach 1 under the map n -> n/3 if n ≡
 *            0 (mod 3) n -> (4n-1)/3 if n ≡ 1 (mod 3) n -> (4n+1)/3 if n ≡ 2
 *            (mod 3) Computing via direct simulation
 * Range:     u8 n <= 67, u16 n <= 7185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    seq_t val = (seq_t)n;      /* current value in the trajectory */
    seq_t steps = 0;           /* number of iterations performed */
    while (val != 1) {
        int rem = (int)(val % (seq_t)3);  /* remainder modulo 3 */
        if (rem == 0) {
            val = val / 3;
        } else if (rem == 1) {
            val = (val * 4 - 1) / 3;
        } else { /* rem == 2 */
            val = (val * 4 + 1) / 3;
        }
        steps++;
    }
    return steps;
}
