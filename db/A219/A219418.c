/*
 * A219418  Unchanging value maps: number of n X 5 binary arrays indicating
 *          the locations of corresponding elements unequal to no horizontal
 *          or antidiagonal neighbor in a random 0..2 n X 5 array.
 * https://oeis.org/A219418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) -17*a(n-2) +117*a(n-3) -355*a(n-4) +550*a(n-5)
 *            -1954*a(n-6) +1579*a(n-7) -1059*a(n-8) -1065*a(n-9)
 *            +1097*a(n-10) +8*a(n-11) -153*a(n-12) +45*a(n-13) -6*a(n-14)
 *            Initial values (n=1..16): a(1)=12, a(2)=160, a(3)=2121,
 *            a(4)=28154, a(5)=372560, a(6)=4934366, a(7)=65352789,
 *            a(8)=86552...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial values for n = 1 .. 16 */
static const seq_t init[17] = {
    0ULL, /* dummy index 0 */
    12ULL,
    160ULL,
    2121ULL,
    28154ULL,
    372560ULL,
    4934366ULL,
    65352789ULL,
    865523957ULL,
    11462977378ULL,
    151815415029ULL,
    2010639231109ULL,
    26628853182336ULL,
    352671832205165ULL,
    4670776468884680ULL,
    61859640739887596ULL,
    819267455394261112ULL
};

/* Cache large enough for the expected maximal n (≈10000). */
#define CACHE_SIZE 10001
static seq_t cache[CACHE_SIZE];
static idx_t max_cached = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    /* One‑time initialization of the cache with the first 16 terms. */
    if (!initialized) {
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = init[i];
        cache[0] = 0;
        max_cached = 16;
        initialized = 1;
    }

    if (n <= max_cached)            /* term already computed */
        return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        if (k <= 16) {              /* safety: re‑assert known terms */
            cache[k] = init[k];
            continue;
        }

        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions */
        pos += 14  * cache[k - 1];
        pos += 117 * cache[k - 3];
        pos += 550 * cache[k - 5];
        pos += 1579 * cache[k - 7];
        pos += 1097 * cache[k -10];
        pos += 8   * cache[k -11];
        pos += 45  * cache[k -13];

        /* Negative contributions */
        neg += 17  * cache[k - 2];
        neg += 355 * cache[k - 4];
        neg += 1954* cache[k - 6];
        neg += 1059* cache[k - 8];
        neg += 1065* cache[k - 9];
        neg += 153 * cache[k -12];
        neg += 6   * cache[k -14];

        cache[k] = pos - neg;        /* final value is non‑negative */
    }

    max_cached = n;
    return cache[n];
}
