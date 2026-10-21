/*
 * A226841  Minimum k such that (F(n) - 1) | sum_{i=n..n+k} F(i), where F(i)
 *          are Fibonacci numbers (A000045).
 * https://oeis.org/A226841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the residue of n modulo 8. * For n = 1 or 2,
 *            a(n) = 1 by convention. * For n ≥ 3: * Let k = n / 8 (integer
 *            division, truncating towards zero), * and r = n % 8 (0 ≤ r ≤ 7).
 *            Then: * r = 0: a = 16 k² * r = 1: a = 32 k² + 8 k – 2 * r = 2: a
 *            = (32 k² + 8 k – 2) / 2 – 2 // uses a(8k+...
 * Range:     u8 n <= 22, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)2)           // n == 1 or 2
        return (seq_t)1;

    idx_t k = n / (idx_t)8;      // floor division
    idx_t r = n % (idx_t)8;      // remainder 0..7

    /* Cast to seq_t before multiplication to avoid overflow of idx_t */
    seq_t kk = (seq_t)k;
    seq_t result;

    switch (r) {
        case 0:   // n = 8k
            result = (seq_t)16 * kk * kk;
            break;
        case 1:   // n = 8k+1
            result = (seq_t)32 * kk * kk + (seq_t)8 * kk - (seq_t)2;
            break;
        case 2:   // n = 8k+2
        {
            seq_t a1 = (seq_t)32 * kk * kk + (seq_t)8 * kk - (seq_t)2; // a(8k+1)
            result = a1 / (seq_t)2 - (seq_t)2;
            break;
        }
        case 3:   // n = 8k+3
            result = (seq_t)32 * kk * kk + (seq_t)24 * kk + (seq_t)2;
            break;
        case 4:   // n = 8k+4
            result = (seq_t)32 * kk * kk + (seq_t)40 * kk + (seq_t)9;
            break;
        case 5:   // n = 8k+5
            result = (seq_t)32 * kk * kk + (seq_t)40 * kk + (seq_t)10; // a(8k+4)+1
            break;
        case 6:   // n = 8k+6
            result = (seq_t)16 * kk * kk + (seq_t)28 * kk + (seq_t)11;
            break;
        case 7:   // n = 8k+7
            result = (seq_t)32 * kk * kk + (seq_t)56 * kk + (seq_t)22;
            break;
        default:  // should never happen
            result = (seq_t)0;
            break;
    }
    return result;
}
