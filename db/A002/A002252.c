/*
 * A002252  Order of letters on standard U.S. typewriter keyboard.
 * https://oeis.org/A002252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = position in alphabet of the n-th key on QWERTY keyboard
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1 || n > 26)
        return 0;
    
    static const char keyboard[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
    
    return (seq_t)(keyboard[n - 1] - 'A' + 1);
}
