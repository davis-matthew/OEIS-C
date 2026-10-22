/*
 * A002806  Number of ways of getting nothing, a pair, 2 pair, 3 of a kind,
 *          other straight, other flush, full house, 4 of a kind, other
 *          straight flush, or a royal flush in 5-card poker.
 * https://oeis.org/A002806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = number of 5-card poker hands of type n, ordered by
 *            frequency
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1 || n > 10) return 0;
    
    switch (n) {
        case 1:  return (seq_t)1302540;        // high card
        case 2:  return (seq_t)(13*6*220*64);  // one pair: C(13,1)*C(4,2)*C(12,3)*C(4,1)^3
        case 3:  return (seq_t)(78*36*11*4);   // two pair: C(13,2)*C(4,2)^2*C(11,1)*C(4,1)
        case 4:  return (seq_t)(13*4*66*16);   // three of a kind: C(13,1)*C(4,3)*C(12,2)*C(4,1)^2
        case 5:  return (seq_t)(10*1024-40);   // straight: 10*4^5-40
        case 6:  return (seq_t)(1287*4-40);    // flush: C(13,5)*4-40
        case 7:  return (seq_t)(13*4*12*6);    // full house: C(13,1)*C(4,3)*C(12,1)*C(4,2)
        case 8:  return (seq_t)(13*48);        // four of a kind: C(13,1)*C(4,4)*C(48,1)
        case 9:  return (seq_t)(40-4);         // straight flush: 10*4-4
        case 10: return (seq_t)4;              // royal flush
    }
    return 0;
}
