/*
 * A002963  Number of chisel strokes required for Roman numerals for n.
 * https://oeis.org/A002963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(n) = sum of stroke counts for Roman numeral representation of
 *            n. Stroke counts: I=1, V=2, X=2, L=2, C=2, D=3, M=4. Uses the
 *            standard Roman numeral algorithm with value-stroke pairs
 * Range:     u8 n <= 3999, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t strokes = 0;
    
    static const struct {
        idx_t value;
        seq_t stroke_count;
    } numerals[] = {
        {1000, 4},   // M
        {900, 6},    // CM (C=2 + M=4)
        {500, 3},    // D
        {400, 5},    // CD (C=2 + D=3)
        {100, 2},    // C
        {90, 4},     // XC (X=2 + C=2)
        {50, 2},     // L
        {40, 4},     // XL (X=2 + L=2)
        {10, 2},     // X
        {9, 3},      // IX (I=1 + X=2)
        {5, 2},      // V
        {4, 3},      // IV (I=1 + V=2)
        {1, 1}       // I
    };
    
    idx_t num = n;
    for (int i = 0; i < 13; i++) {
        while (num >= numerals[i].value) {
            strokes += numerals[i].stroke_count;
            num -= numerals[i].value;
        }
    }
    
    return strokes;
}
