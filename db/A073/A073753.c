/*
 * A073753  a(n) = A073752(A073752(n)), where A073752(n) = gcd(n/lpf(n),
 *          n/gpf(n)), with lpf(n) as the least and gpf(n) as the greatest
 *          prime factor of n (see A020639, A006530).
 * https://oeis.org/A073753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A073752(A073752(n)) where A073752(m) = gcd(m/lpf(m),
 *            m/gpf(m)), lpf=least prime factor, gpf=greatest prime factor
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_a073752_uint64, seq_gpf_uint64, seq_local_gcd,
 *            seq_lpf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_a073752_uint64.h"
#include "../utils/seq_gpf_uint64.h"
#include "../utils/seq_local_gcd.h"
#include "../utils/seq_lpf_uint64.h"

/* ---------- local helpers ------------------------------------------------- */

/* ---------- main sequence function -------------------------------------- */

seq_t a(idx_t n)
{
    /* first pass */
    seq_t v1 = seq_a073752_uint64((uint64_t)n);

    /* second pass: n >= 1, v1 fits in seq_t, convert to uint64_t safely */
    return seq_a073752_uint64((uint64_t)v1);
}
