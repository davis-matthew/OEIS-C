/*
 * A057536  Minimal number of coins needed to pay n Euro-cents using the Euro
 *          currency.
 * https://oeis.org/A057536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimal number of euro coins (denominations:
 *            1,2,5,10,20,50,100, 200,500,1000,2000,5000,10000,20000,50000)
 *            needed to sum to n cents
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t coin[] = {
        1, 2, 5, 10, 20, 50, 100, 200, 500,
        1000, 2000, 5000, 10000, 20000, 50000
    };
    seq_t res = 0;
    idx_t r = n;
    for (int i = (int)(sizeof(coin)/sizeof(coin[0])) - 1; i >= 0; --i) {
        seq_t q = (seq_t)(r / coin[i]);   /* how many coins of this value */
        res += q;
        r %= coin[i];                     /* remainder after using these coins */
    }
    return res;
}
