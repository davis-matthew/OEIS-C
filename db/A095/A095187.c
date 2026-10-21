/*
 * A095187  Least significant digit of (n mod 10)^floor(n/10).
 * https://oeis.org/A095187
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n mod 10)^(floor(n/10)) mod 10
 * Range:     u8 n <= 129, u16 n <= 129, u32 n <= 129, u64 n <= 129,
 *            u128 n <= 129, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the least significant decimal digit of (n mod 10)^⌊n/10⌋. */

seq_t a(idx_t n)
{
    /* n >= 10 (offset) is guaranteed by the caller, but we handle n < 10 just in case */
    if (n < 10) {
        /* For indices below the defined offset, return 0 (arbitrary but harmless) */
        return (seq_t)0;
    }

    /* Exponent is floor(n/10) */
    idx_t exp = n / 10;
    unsigned int base = n % 10;

    /* Compute base^exp mod 10 using periodicity of powers modulo 10 */
    switch (base) {
        case 0: return (seq_t)0;            /* 0^k mod 10 = 0 for k>=1 */
        case 1: return (seq_t)1;            /* 1^k mod 10 = 1 */
        case 5: return (seq_t)5;            /* 5^k mod 10 = 5 */
        case 6: return (seq_t)6;            /* 6^k mod 10 = 6 */

        case 2: {          /* cycle: 2,4,8,6 (period 4) */
            unsigned r = exp % 4;
            return (seq_t)(r == 0 ? 6 : r == 1 ? 2 : r == 2 ? 4 : 8);
        }
        case 3: {          /* cycle: 3,9,7,1 (period 4) */
            unsigned r = exp % 4;
            return (seq_t)(r == 0 ? 1 : r == 1 ? 3 : r == 2 ? 9 : 7);
        }
        case 4: {          /* cycle: 4,6 (period 2) */
            unsigned r = exp % 2;
            return (seq_t)(r == 0 ? 6 : 4);
        }
        case 7: {          /* cycle: 7,9,3,1 (period 4) */
            unsigned r = exp % 4;
            return (seq_t)(r == 0 ? 1 : r == 1 ? 7 : r == 2 ? 9 : 3);
        }
        case 8: {          /* cycle: 8,4,2,6 (period 4) */
            unsigned r = exp % 4;
            return (seq_t)(r == 0 ? 6 : r == 1 ? 8 : r == 2 ? 4 : 2);
        }
        case 9: {          /* cycle: 9,1 (period 2) */
            unsigned r = exp % 2;
            return (seq_t)(r == 0 ? 1 : 9);
        }

        default:           /* unreachable for base 0-9 */
            return (seq_t)0;
    }
}
