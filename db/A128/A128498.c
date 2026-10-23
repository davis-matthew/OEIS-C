/*
 * A128498  Fourth column (m=3) of triangle A128494.
 * https://oeis.org/A128498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{j=0}^{floor(n/2)} (-1)^j * binom(j+3,3)
 * Range:     i8 n <= 19, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t binom_j3(idx_t j)
{
    seq_t a = (seq_t)(j + 3);
    seq_t b = (seq_t)(j + 2);
    seq_t c = (seq_t)(j + 1);
    return (a * b * c) / 6;
}

#if defined(__cplusplus)

// C++ implementation using std::vector
#include <vector>

static std::vector<seq_t> cacheCPP;
static seq_t current_sumCPP = 0;

seq_t a(idx_t n)
{
    if (cacheCPP.empty()) {
        cacheCPP.resize(2);
        cacheCPP[0] = (seq_t)1;
        cacheCPP[1] = (seq_t)1;
        current_sumCPP = (seq_t)1;
    }
    while (cacheCPP.size() <= n) {
        idx_t j = cacheCPP.size() / 2;
        seq_t sign = ((j & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        seq_t term = sign * binom_j3(j);
        current_sumCPP += term;
        cacheCPP.push_back(current_sumCPP);
        cacheCPP.push_back(current_sumCPP);
    }
    return cacheCPP[n];
}

#else

// C implementation using malloc/realloc
static seq_t *cacheC = NULL;
static idx_t cache_sizeC = 0;
static idx_t cache_capC = 0;
static seq_t current_sumC = 0;

seq_t a(idx_t n)
{
    if (cacheC == NULL) {
        cache_capC = 2;
        cacheC = (seq_t *)malloc(sizeof(seq_t) * cache_capC);
        if (!cacheC) abort();
        cacheC[0] = (seq_t)1;
        cacheC[1] = (seq_t)1;
        cache_sizeC = 2;
        current_sumC = (seq_t)1;
    }

    while (cache_sizeC <= n) {
        idx_t j = cache_sizeC / 2;
        seq_t sign = ((j & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        seq_t term = sign * binom_j3(j);
        current_sumC += term;

        if (cache_sizeC + 2 > cache_capC) {
            size_t new_cap = cache_capC * 2;
            if (new_cap < (size_t)(cache_sizeC + 2)) new_cap = cache_sizeC + 2;
            seq_t *new_ptr = (seq_t *)realloc(cacheC, sizeof(seq_t) * new_cap);
            if (!new_ptr) abort();
            cacheC = new_ptr;
            cache_capC = (idx_t)new_cap;
        }

        cacheC[cache_sizeC] = current_sumC;
        cacheC[cache_sizeC + 1] = current_sumC;
        cache_sizeC += 2;
    }
    return cacheC[n];
}

#endif
