/*
 * A094087  Domination number of the Cartesian product of two n-cycles.
 * https://oeis.org/A094087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dominating number of the Cartesian product of two n‑cycles (torus grid graph).
   Formula (for n ≥ 6):
   let k = n / 5 (integer division) and r = n % 5
     r == 0 : a(n) = 5 k²
     r == 1 : a(n) = 5 k² + 4 k – 1
     r == 2 : a(n) = 5 k² + 6 k + (k odd && k < 5 ? 1 : 0)
     r == 3 : a(n) = 5 k² + 8 k + 2 + (k == 1 ? 1 : 0) – (k ≥ 5 ? 1 : 0)
     r == 4 : a(n) = 5 k² + 9 k + 4
   The first five terms are handled explicitly. */

/* If the required value does not fit in seq_t, the arithmetic
   automatically promotes to an arbitrary-precision integer
   when appropriate.  The function uses only integral
   arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Small indices, just return the given values. */
    if (n <= 5) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            case 4: return (seq_t)4;
            case 5: return (seq_t)5;
            default: return (seq_t)0; /* unreachable */
        }
    }

    idx_t k = n / 5;
    seq_t sk = (seq_t)k;
    idx_t r = n % 5;
    seq_t result;

    switch (r) {
        case 0:   /* n ≡ 0 (mod 5) */
            result = (seq_t)5 * sk * sk;
            break;
        case 1:   /* n ≡ 1 (mod 5) */
            result = (seq_t)5 * sk * sk + (seq_t)4 * sk - (seq_t)1;
            break;
        case 2:   /* n ≡ 2 (mod 5) */
            result = (seq_t)5 * sk * sk + (seq_t)6 * sk;
            if ((sk & (seq_t)1) && k < 5)
                result += (seq_t)1;
            break;
        case 3:   /* n ≡ 3 (mod 5) */
            result = (seq_t)5 * sk * sk + (seq_t)8 * sk + (seq_t)2;
            if (k == 1)
                result += (seq_t)1;
            if (k >= 5)
                result -= (seq_t)1;
            break;
        case 4:   /* n ≡ 4 (mod 5) */
            result = (seq_t)5 * sk * sk + (seq_t)9 * sk + (seq_t)4;
            break;
        default:
            result = (seq_t)0; /* unreachable */
            break;
    }

    return result;
}
