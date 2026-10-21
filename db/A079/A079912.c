/*
 * A079912  Solution to the Dancing School Problem with 7 girls and n+7 boys:
 *          f(7,n).
 * https://oeis.org/A079912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 - 14*n^6 + 126*n^5 - 700*n^4 + 2625*n^3 - 6342*n^2 +
 *            9072*n - 5840 for n > 4 Initial values: a(0)=1, a(1)=8,
 *            a(2)=133, a(3)=1044, a(4)=5794
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 567, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4) {
        switch (n) {
            case 0: return 1;
            case 1: return 8;
            case 2: return 133;
            case 3: return 1044;
            case 4: return 5794;
        }
    }
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;          // n^2
    seq_t n3 = n2 * s;         // n^3
    seq_t n4 = n3 * s;         // n^4
    seq_t n5 = n4 * s;         // n^5
    seq_t n6 = n5 * s;         // n^6
    seq_t n7 = n6 * s;         // n^7

    seq_t res = n7;
    res -= 14 * n6;   // -(14*n^6)
    res += 126 * n5;  // +126*n^5
    res -= 700 * n4;  // -700*n^4
    res += 2625 * n3; // +2625*n^3
    res -= 6342 * n2; // -6342*n^2
    res += 9072 * s;  // +9072*n
    res -= 5840;      // -5840

    return res;
}
