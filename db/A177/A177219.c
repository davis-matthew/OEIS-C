/*
 * A177219  a(1) = 1; a(2n) = -a(n); a(2n+1) = -a(n) + a(n+1).
 * https://oeis.org/A177219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1; a(2n) = -a(n); a(2n+1) = -a(n) + a(n+1)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static tables holding computed values and a flag that marks validity.  */
static seq_t *cache = NULL;          /* Holds a(i) for i <= cap-1 */
static unsigned char *valid = NULL;  /* 0 = not computed, 1 = valid */
static size_t cap = 0;               /* Current capacity of the tables */

/* Ensure that the tables are large enough for index n.  */
static void ensure_cap(idx_t n) {
    if (n >= cap) {
        size_t newcap = cap ? (cap << 1) : 1;
        while (newcap <= n) newcap <<= 1;

        seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
        unsigned char *newvalid = (unsigned char *)malloc(newcap);

        if (cache) {
            memcpy(newcache, cache, cap * sizeof(seq_t));
            memcpy(newvalid, valid, cap);
            free(cache);
            free(valid);
        }
        /* Mark new elements as invalid. */
        memset(newvalid + cap, 0, (newcap - cap) * sizeof(unsigned char));

        cache = newcache;
        valid = newvalid;
        cap = newcap;
    }
}

/* Recursive computation that assumes the table has sufficient capacity.  */
static seq_t rec(idx_t m) {
    if (valid[m])               /* already computed */
        return cache[m];

    seq_t val;
    if (m == 1) {
        val = 1;
    } else if ((m & (idx_t)1) == 0) {            /* even */
        val = -rec(m >> 1);
    } else {                                     /* odd */
        idx_t k = m >> 1;
        val = -rec(k) + rec(k + 1);
    }

    cache[m] = val;
    valid[m] = 1;
    return val;
}

/* Public entry point required by the testing harness.  */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;               /* undefined behavior for n < 1, but harmless */
    ensure_cap(n);
    return rec(n);
}
