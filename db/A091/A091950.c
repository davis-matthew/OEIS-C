/*
 * A091950  a(n) = A087659(n) mod 3.
 * https://oeis.org/A091950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A087659(n) mod 3; a(9k)=1, a(9k+1)=0, a(9k+2)=0,
 *            a(9k+3)=2, a(9k+4)=1, a(9k+5)=1, a(9k+6)=2*A014578(k+1),
 *            a(9k+7)=a(9k+8)=z(k), with z(9k)=1, z(9k+1)=2, z(9k+2)=0,
 *            z(9k+3)=1, z(9k+4)=2, z(9k+5)=1, z(9k+6)=1, z(9k+7)=2,
 *            z(9k+8)=z(k)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A014578_a(idx_t n)
{
    if (n == 0)
        return 0;
    int flip = 0;
    while (n % 3 == 0) {
        n /= 3;
        flip ^= 1;
    }
    return flip ? 0 : 1;
}

static int z_val(idx_t k)
{
    for (;;) {
        unsigned r = (unsigned)(k % 9);
        switch (r) {
            case 0: return 1;
            case 1: return 2;
            case 2: return 0;
            case 3: return 1;
            case 4: return 2;
            case 5: return 1;
            case 6: return 1;
            case 7: return 2;
            default: k = k / 9; break; /* r == 8: z(9k+8) = z(k) */
        }
    }
}

seq_t a(idx_t n)
{
    idx_t k = n / 9;
    unsigned r = (unsigned)(n % 9);
    switch (r) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)2;
        case 4: return (seq_t)1;
        case 5: return (seq_t)1;
        case 6: return (seq_t)(2 * A014578_a(k + 1));
        default: return (seq_t)z_val(k);
    }
}
