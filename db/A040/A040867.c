/*
 * A040867  Continued fraction for sqrt(897).
 * https://oeis.org/A040867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=29; a(1)=1; a(2)=18; a(3)=1; a(4)=58; a(n)=a(n-4) for n>4
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)29;
    }
    switch ((n % 4)) {
        case 0: return (seq_t)58; // n ≡ 0 (mod 4), n≥4
        case 1: return (seq_t)1;  // n ≡ 1 (mod 4), n≥1
        case 2: return (seq_t)18; // n ≡ 2 (mod 4), n≥2
        case 3: return (seq_t)1;  // n ≡ 3 (mod 4), n≥3
        default: return (seq_t)0; // unreachable
    }
}
