/*
 * A003678  Decimal expansion of the SI unit c (speed of light in vacuum), c =
 *          299792458 meters/second.
 * https://oeis.org/A003678
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   decimal expansion of 299792458
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 9) return 0;
    
    idx_t pos = n - 9;
    
    if (pos >= 9) return 0;
    
    uint64_t value = 299792458;
    uint64_t divisor = 1;
    for (idx_t i = 0; i < 8 - pos; i++) {
        divisor *= 10;
    }
    
    return (seq_t)((value / divisor) % 10);
}
