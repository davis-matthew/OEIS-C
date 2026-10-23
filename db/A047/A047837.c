/*
 * A047837  Honaker's triangle problem: form a triangle with base of length n,
 *          all entries different, all row sums equal; a(n) gives minimal row
 *          sum.
 * https://oeis.org/A047837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047837 – Honaker's triangle.
   The sequence follows a 16‑term linear recurrence with constants:
   a(1)=1, a(2)=3, a(3)=8, a(4)=15, a(5)=27, a(6)=43, a(7)=65,
   a(8)=94, a(9)=130, a(10)=175, a(11)=229, a(12)=294, a(13)=369,
   a(14)=456, a(15)=557, a(16)=671.
   For n>16:
     a(n) =  3 a(n-1) - 4 a(n-2) + 5 a(n-3) - 7 a(n-4) + 8 a(n-5)
            - 8 a(n-6) + 8 a(n-7) - 8 a(n-8) + 8 a(n-9) - 8 a(n-10)
            + 8 a(n-11) - 7 a(n-12) + 5 a(n-13) - 4 a(n-14) + 3 a(n-15)
            -    a(n-16);
*/

#include <stdlib.h>

static const seq_t init_terms[16] = {
    (seq_t)1,  (seq_t)3,  (seq_t)8,  (seq_t)15,
    (seq_t)27, (seq_t)43, (seq_t)65, (seq_t)94,
    (seq_t)130,(seq_t)175,(seq_t)229,(seq_t)294,
    (seq_t)369,(seq_t)456,(seq_t)557,(seq_t)671
};

static const int rec[16] = {
    3,  -4,  5,  -7,  8,  -8,  8,  -8,
    8,  -8,  8,  -7,  5,  -4,  3,  -1
};

seq_t a(idx_t n)
{
    if (n <= 16)
        return init_terms[n - 1];

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t csize = 0;   /* number of terms stored */

    /* Allocate or grow cache if necessary */
    if (cache == NULL) {
        cap = 32;
        if (cap < n)
            cap = n;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();           /* cannot happen in the test harness */
    } else if ((idx_t)n > cap) {
        idx_t newcap = cap << 1;
        if (newcap < n)
            newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise first 16 values if not already done */
    if (csize < 16) {
        for (idx_t i = csize; i < 16; ++i)
            cache[i] = init_terms[i];
        csize = 16;
    }

    /* Compute new terms up to n */
    for (idx_t i = csize; i < n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 16; ++k) {
            seq_t prod = cache[i - 1 - (idx_t)k] *
                         (seq_t)(rec[k] > 0 ? rec[k] : -rec[k]);
            if (rec[k] > 0)
                pos += prod;
            else
                neg += prod;
        }
        cache[i] = pos - neg;
    }

    csize = n;
    return cache[n - 1];
}
