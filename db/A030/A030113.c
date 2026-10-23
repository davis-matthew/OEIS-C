/*
 * A030113  Number of antichains in the product of a fence poset with n
 *          elements and a chain with 8 elements; also number of paths with n
 *          turns when light is reflected from 9 glass plates.
 * https://oeis.org/A030113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9, a(2)=45, a(3)=285, a(4)=1695, a(5)=10317,
 *            a(6)=62349, a(7)=377739, a(8)=2286648. For n≥9:
 *            a(n)=5·a(n-1)+10·a(n-2)-20·a(n-3)-15·a(n-4)+21·a(n-5)+7·a(n-6)
 *            -8·a(n-7)-a(n-8)+a(n-9)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for computed values */
    static seq_t *seq = NULL;          /* array holding a(0) … a(max_computed) */
    static idx_t seq_len = 0;          /* allocated length of seq array   */
    static idx_t max_computed = 0;     /* largest index for which a(index) is stored */

    /* initial allocation and seed values */
    if (seq == NULL) {
        seq_len = 9;                     /* store a(0) … a(8) at least */
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_len);
        if (seq == NULL) return (seq_t)0;/* out of memory: return 0 (should not happen) */

        seq[0] = (seq_t)1;
        seq[1] = (seq_t)9;
        seq[2] = (seq_t)45;
        seq[3] = (seq_t)285;
        seq[4] = (seq_t)1695;
        seq[5] = (seq_t)10317;
        seq[6] = (seq_t)62349;
        seq[7] = (seq_t)377739;
        seq[8] = (seq_t)2286648;
        max_computed = 8;
    }

    /* enlarge storage if necessary */
    if ((idx_t)(n + 1) > seq_len) {
        idx_t new_len = (idx_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq, sizeof(seq_t) * new_len);
        if (tmp == NULL) return (seq_t)0;/* out of memory: return 0 (should not happen) */
        seq = tmp;
        seq_len = new_len;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence written without negative constants to stay in unsigned arithmetic */
        seq_t val = 0;

        /* positive part: 5*a(i-1)+10*a(i-2)+21*a(i-5)+7*a(i-6)+a(i-9) */
        val += (seq_t)5   * seq[i - 1];
        val += (seq_t)10  * seq[i - 2];
        val += (seq_t)21  * seq[i - 5];
        val += (seq_t)7   * seq[i - 6];
        val +=            seq[i - 9];

        /* negative part: 20*a(i-3)+15*a(i-4)+8*a(i-7)+a(i-8) */
        val -= (seq_t)20  * seq[i - 3];
        val -= (seq_t)15  * seq[i - 4];
        val -= (seq_t)8   * seq[i - 7];
        val -=            seq[i - 8];

        seq[i] = val;
    }

    max_computed = n > max_computed ? n : max_computed;
    return seq[n];
}
