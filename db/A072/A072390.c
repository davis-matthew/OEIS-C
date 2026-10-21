/*
 * A072390  Sum of two powers of 13.
 * https://oeis.org/A072390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n‑th smallest value of 13^i + 13^j with i,j ≥ 0
 *            (unordered pairs)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 44, u64 n <= 170, u128 n <= 629,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the algorithm generates all needed powers of 13 up to the point
   where at least n+1 distinct sums can be formed.  All arithmetic is
   performed with the native type seq_t, and overflow during multiplication
   or addition is detected by the usual unsigned wrap–around test
   (sum < operand or product < operand).  This guarantees that the
   sequence is correct for native unsigned integer types while still
   working for arbitrary‑precision classes. */
#include <stdlib.h>
#include <stdint.h>

static int cmp_seq_t(const void *a, const void *b) {
    const seq_t v1 = *(const seq_t *)a;
    const seq_t v2 = *(const seq_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

seq_t a(idx_t n) {
    /* We need powers of 13 up to the point where the number of
       unordered pairs (i,j) is at least n+1.  Since the
       number of ordered pairs satisfies (k+1)(k+2)/2 where k is the
       largest exponent, we pre‑allocate enough space.  For n up to
       ~10000 the exponent k is < 150, so 200 is safe. */
    const size_t MAX_POW = 200;
    seq_t powers[MAX_POW];
    size_t pow_cnt = 1;          /* 13^0 */
    powers[0] = (seq_t)1;
    /* Compute subsequent powers until multiplication would overflow
       or until we have generated enough pairs.  We stop earlier if we
       already know the required number of terms cannot be obtained
       because of overflow (typical for small seq_t).               */
    for (size_t i = 1; i < MAX_POW; ++i) {
        seq_t next = powers[i-1] * (seq_t)13;
        if (next < powers[i-1])                 /* unsigned overflow */
            break;
        powers[i] = next;
        ++pow_cnt;
    }
    /* Size needed for all unordered pairs (i,j) with i <= j < pow_cnt */
    size_t terms_cap = pow_cnt * (pow_cnt + 1) / 2;
    seq_t *terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) return 0;                        /* allocation failure */
    size_t terms_cnt = 0;
    for (size_t i = 0; i < pow_cnt; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            seq_t sum = powers[i] + powers[j];
            if (sum < powers[i])                /* addition overflow */
                continue;
            terms[terms_cnt++] = sum;
        }
    }
    /* Sort the sums and remove duplicates (unlikely but harmless). */
    qsort(terms, terms_cnt, sizeof(seq_t), cmp_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < terms_cnt; ++i) {
        if (uniq == 0 || terms[i] != terms[uniq-1]) {
            terms[uniq++] = terms[i];
        }
    }
    seq_t result = 0;
    if ((idx_t)uniq > n) result = terms[n];
    free(terms);
    return result;
}
