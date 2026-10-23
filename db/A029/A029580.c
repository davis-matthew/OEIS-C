/*
 * A029580  a(0) = 1; for n > 0, a(n) = [ A*a(n-1)+B ]/p^r, where p^r is the
 *          highest power of p dividing [ A*a(n)+B ] and p=2, A=2.00013,
 *          B=3.0.
 * https://oeis.org/A029580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 3680, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;  
   for n > 0,
   let T = floor( (200013 * a(n-1) + 300000) / 100000 ) = 2*a(n-1) + 3 + floor(13*a(n-1)/100000)  
   let r = v2(T)  (the exponent of the highest power of 2 dividing T)  
   then a(n) = T >> r  (divide T by 2^r) */
seq_t a(idx_t n)
{
    seq_t cur = 1;
    if (n == 0) return cur;

    const seq_t two   = (seq_t)2;
    const seq_t three = (seq_t)3;
    const seq_t thirteen = (seq_t)13;
    const seq_t onehundredk = (seq_t)100000;

    for (idx_t i = 1; i <= n; ++i) {
        /* T = 2*cur + 3 + floor(13*cur / 100000) */
        seq_t s = (cur * thirteen) / onehundredk;
        seq_t T = cur * two + three + s;

        /* remove all factors of 2 from T */
        while ((T & (seq_t)1) == 0) {
            T >>= 1;
        }
        cur = T;
    }
    return cur;
}
