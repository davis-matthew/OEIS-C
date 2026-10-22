/*
 * A003899  Degrees of irreducible representations of group L3(9).
 * https://oeis.org/A003899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of the irreducible characters of L3(9) = PSL(3,9) =
 *            PGL(3,9) (gcd(3,q-1)=1 for q=9), listed in ascending order with
 *            multiplicity. From the unipotent/Deligne-Lusztig families of
 *            GL(3,q) restricted to characters trivial on the center
 *            (multiplicity divided by q-1): 1 (x1), q^2+q (x1), q^2+q+...
 * Range:     u8 n <= 9, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const long long q = 9;
    long long deg[8], mult[8];
    idx_t k = n;
    int i;

    deg[0] = 1;                         mult[0] = 1;
    deg[1] = q * q + q;                 mult[1] = 1;
    deg[2] = q * q + q + 1;             mult[2] = q - 2;
    deg[3] = (q - 1) * (q - 1) * (q + 1); mult[3] = q * (q + 1) / 3;
    deg[4] = (q - 1) * (q * q + q + 1); mult[4] = q * (q - 1) / 2;
    deg[5] = q * q * q;                 mult[5] = 1;
    deg[6] = q * (q * q + q + 1);       mult[6] = q - 2;
    deg[7] = (q + 1) * (q * q + q + 1); mult[7] = (q - 2) * (q - 3) / 6;

    if (n < 1) return (seq_t)0;
    for (i = 0; i < 8; i++) {
        if (k <= (idx_t)mult[i]) return (seq_t)deg[i];
        k -= (idx_t)mult[i];
    }
    return (seq_t)0;
}
