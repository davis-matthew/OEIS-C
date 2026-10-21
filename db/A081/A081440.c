/*
 * A081440  Expansion of e.g.f.: exp(x)*cosh(x/sqrt(1 - x^2)).
 * https://oeis.org/A081440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A081440: a(n) = expansion of exp(x)*cosh(x/sqrt(1 - x^2))
   Recurrence (n ≥ 8):
     a(n) = 2*a(n-1) + 3*(n-2)^2*a(n-2)
            - 3*(n-2)*(2*n-5)*a(n-3)
            - 3*(n-3)*(n-2)*(n^2-7*n+11)*a(n-4)
            + 6*(n-4)^2*(n-3)*(n-2)*a(n-5)
            + (n-7)*(n-5)*(n-4)*(n-3)^2*(n-2)*a(n-6)
            - (n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(2*n-11)*a(n-7)
            + (n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)*a(n-8);
   Initial values:
     a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=20, a(5)=76, a(6)=632, a(7)=3424. */

seq_t a(idx_t n) {
    static const seq_t init[8] = {1, 1, 2, 4, 20, 76, 632, 3424};
    if (n <= 7) return init[n];

    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i) buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t ns  = (seq_t)i;
        seq_t nm2 = ns - 2;
        seq_t nm3 = ns - 3;
        seq_t nm4 = ns - 4;
        seq_t nm5 = ns - 5;
        seq_t nm6 = ns - 6;
        seq_t nm7 = ns - 7;

        seq_t t  = 2 * buf[(i - 1) % 8];
        t += 3 * nm2 * nm2 * buf[(i - 2) % 8];
        t -= 3 * nm2 * (2 * ns - 5) * buf[(i - 3) % 8];
        t -= 3 * nm3 * nm2 * (ns * ns - 7 * ns + 11) * buf[(i - 4) % 8];
        t += 6 * nm4 * nm4 * nm3 * nm2 * buf[(i - 5) % 8];
        t += nm7 * nm5 * nm4 * nm3 * nm3 * nm2 * buf[(i - 6) % 8];
        t -= nm6 * nm5 * nm4 * nm3 * nm2 * (2 * ns - 11) * buf[(i - 7) % 8];
        t += nm7 * nm6 * nm5 * nm4 * nm3 * nm2 * buf[(i - 8) % 8];

        buf[i % 8] = t;
    }

    return buf[n % 8];
}
