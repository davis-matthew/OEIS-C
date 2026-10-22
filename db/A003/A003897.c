/*
 * A003897  Degrees of irreducible representations of group L3(7).
 * https://oeis.org/A003897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   finite list (full). The irreducible character degrees of L3(q) =
 *            PSL(3,q) for q = 7 (q == 1 mod 3), in ascending order with
 *            multiplicities: 1, q(q+1), q^2+q+1, (q+1)(q^2+q+1)/3 [x3],
 *            (q-1)^2(q+1) [x6], (q-1)(q^2+q+1) [x7], q^3, q(q^2+q+1),
 *            (q+1)(q^2+q+1). The 22 degrees match the 22 conjugacy cla...
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char A003897_mult[9] = {1, 1, 1, 3, 6, 7, 1, 1, 1};

seq_t a(idx_t n)
{
    seq_t q = 7;
    seq_t d[9];
    idx_t k;
    int i;

    d[0] = 1;
    d[1] = q * (q + 1);
    d[2] = q * q + q + 1;
    d[3] = (q + 1) * (q * q + q + 1) / 3;
    d[4] = (q - 1) * (q - 1) * (q + 1);
    d[5] = (q - 1) * (q * q + q + 1);
    d[6] = q * q * q;
    d[7] = q * (q * q + q + 1);
    d[8] = (q + 1) * (q * q + q + 1);

    if (n < 1) return 0;
    k = n - 1;
    for (i = 0; i < 9; i++) {
        if (k < (idx_t)A003897_mult[i]) return d[i];
        k -= (idx_t)A003897_mult[i];
    }
    return 0;
}
