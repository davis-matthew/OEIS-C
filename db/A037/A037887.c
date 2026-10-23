/*
 * A037887  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*10^i} is the base 10
 *          representation of n and e(i) are the digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2)*sum_{i} |d(i)-e(i)|, where d(i) are the digits of n
 *            in base 10 (units digit is d(0)) and e(i) are the digits sorted
 *            in nondecreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence is undefined for n=0, return 0 for completeness */
    if (n == 0) return 0;

    /* maximum number of decimal digits for a uint64_t is 20 */
    unsigned char unsorted[20];
    unsigned char sorted[20];
    idx_t t = n;
    unsigned len = 0;

    /* extract digits (least significant first) */
    while (t > 0) {
        unsorted[len] = (unsigned char)(t % 10);
        t /= 10;
        len++;
    }

    /* copy to another array that we will sort */
    for (unsigned i = 0; i < len; ++i) sorted[i] = unsorted[i];

    /* insertion sort the copy ascending */
    for (unsigned i = 1; i < len; ++i) {
        unsigned char key = sorted[i];
        int j = (int)i - 1;
        while (j >= 0 && sorted[j] > key) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }

    /* compute the sum of absolute differences */
    unsigned sum = 0;
    for (unsigned i = 0; i < len; ++i) {
        unsigned diff = (unsorted[i] > sorted[i]) ?
                          (unsorted[i] - sorted[i]) :
                          (sorted[i] - unsorted[i]);
        sum += diff;
    }

    /* the result is always an integer because the sum is even */
    return (seq_t)(sum / 2);
}
