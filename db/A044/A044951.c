/*
 * A044951  Numbers having a different number of ones and zeros in base 2.
 * https://oeis.org/A044951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer with a different number of ones and zeros *
 *            in its binary representation. (All such integers are *
 *            enumerated starting with 1; the offset is 1.) * * The
 *            implementation uses only standard integer arithmetic. For each *
 *            candidate integer it counts the number of 1‑bits and the tot...
 * Range:     u8 n <= 206, u16 n <= 9754, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   bit_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t and related types */
#include "../utils/bit_count_u64.h"

/* external types (provided by the build system) */
typedef uint64_t idx_t;  /* index type (unsigned 64‑bit) */

/* The sequence value type is supplied by the build environment.  We
 * assume it can be constructed by casting from an idx_t value. */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------ */

/* Return the position of the most significant set bit + 1
 * (i.e. the bit length of x).  x is guaranteed to be >0. */
static unsigned bit_length_u64(idx_t x)
{
    unsigned len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

/* ------------------------------------------------------------ */
/* Main function:  returns a(n) for n >= 1.  For n == 0 an implementation
 * independent value (here 0) is returned.  The function is fully
 * self‑contained and works for any seq_t type as long as the cast
 * (seq_t)k is valid for the result. */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for offset 1; return a harmless value */
        return (seq_t)0;

    idx_t count = 0;   /* how many qualifying numbers have been seen   */
    idx_t k = 1;       /* current candidate integer                   */

    while (1) {
        /* Count ones and total bit length of k. */
        unsigned ones   = bit_count_u64(k);
        unsigned bitlen = bit_length_u64(k);
        unsigned zeros  = bitlen - ones;

        if (ones != zeros) {           /* unequal numbers of 1's and 0's */
            count++;
            if (count == n)
                return (seq_t)k;        /* found the nth term  */
        }

        k++;                            /* next candidate number       */
    }
}
