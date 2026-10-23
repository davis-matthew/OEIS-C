/*
 * A168224  Where record values occur in A168223.
 * https://oeis.org/A168224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n≥2: * a(n) = 5 + 12⋅⌊(n‑2)/3⌋ + {0,2,6}[ (n‑2) mod
 *            3 ]
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _offsets[3] = {0, 2, 6};

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    idx_t k = n - 2;             /* number of steps after the first term */
    idx_t q = k / 3;             /* full 2‑4‑6 cycles */
    idx_t r = k % 3;             /* position inside the current cycle */

    seq_t res = (seq_t)5 + (seq_t)q * (seq_t)12 + (seq_t)_offsets[r];
    return res;
}
