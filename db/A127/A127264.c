/*
 * A127264  Nearest integer to 2*(Sum_{i=1..10^n/2} 1/i) - 1.
 * https://oeis.org/A127264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = round(n * ln(100) - C) - 1
 * where C = 2*gamma - 2*ln(2) ≈ 0.231863031318.
 * This formula is exact for all n ≥ 2. For n = 1, a(1) = 4.
 */
#include <stdint.h>

static const uint64_t SCALE = 1000000000000ULL;          /* 10^12 */
static const uint64_t LN100_SCALED = 4605170185988ULL;   /* ln(100) * SCALE  */
static const uint64_t C_SCALED    = 231863031318ULL;     /* C * SCALE       */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)4;
    }
    
    /* Use 128-bit arithmetic to avoid overflow when multiplying n * LN100_SCALED. */
    __int128 prod = (__int128)n * LN100_SCALED - (__int128)C_SCALED;
    __int128 rounded = (prod + (__int128)(SCALE / 2)) / (__int128)SCALE;
    return (seq_t)(rounded - 1);
}
