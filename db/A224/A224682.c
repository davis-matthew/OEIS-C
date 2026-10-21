/*
 * A224682  Number of (n+4)X(n+4) 0..1 matrices with each 5X5 subblock
 *          idempotent.
 * https://oeis.org/A224682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) –15*a(n-2)+20*a(n-3) –15*a(n-4)+9*a(n-5)
 *            –16*a(n-6)+30*a(n-7) –30*a(n-8)+15*a(n-9) –6*a(n-10)+12*a(n-11)
 *            –18*a(n-12)+12*a(n-13) –3*a(n-14)+1*a(n-15) –3*a(n-16)+3*a(n-17)
 *            –1*a(n-18) for n>22 a(1)=5682, a(2)=1312, a(3)=1636, a(4)=2166,
 *            a(5)=2700, a(6)=3158, a(7)=4572, a(8)=7006, a(9...
 * Range:     u8 none, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache of computed terms; index 0 is unused */
static seq_t cache[20001];
static size_t cache_cap = 0;

/* initialise the cache with the first 22 terms */
static void init_cache(void)
{
    if (cache_cap > 0)
        return;                    /* already initialised */

    for (size_t i = 0; i < 20001; ++i)
        cache[i] = 0;                /* default values */

    static const seq_t init_vals[23] = {
        (seq_t)0,          /* a(0) unused */
        (seq_t)5682,       /* a(1) */
        (seq_t)1312,       /* a(2) */
        (seq_t)1636,       /* a(3) */
        (seq_t)2166,       /* a(4) */
        (seq_t)2700,       /* a(5) */
        (seq_t)3158,       /* a(6) */
        (seq_t)4572,       /* a(7) */
        (seq_t)7006,       /* a(8) */
        (seq_t)10366,      /* a(9) */
        (seq_t)14544,      /* a(10) */
        (seq_t)19462,      /* a(11) */
        (seq_t)26588,      /* a(12) */
        (seq_t)37294,      /* a(13) */
        (seq_t)52826,      /* a(14) */
        (seq_t)74314,      /* a(15) */
        (seq_t)102814,     /* a(16) */
        (seq_t)141460,     /* a(17) */
        (seq_t)195148,     /* a(18) */
        (seq_t)270382,     /* a(19) */
        (seq_t)375154,     /* a(20) */
        (seq_t)518878,     /* a(21) */
        (seq_t)715182      /* a(22) */
    };
    for (size_t i = 0; i < 23; ++i)
        cache[i] = init_vals[i];

    cache_cap = 23;                 /* indices 0..22 now defined */
}

/* extend the cache up to index n (inclusive) if necessary */
static void extend_cache(idx_t n)
{
    init_cache();

    if ((size_t)n + 1 <= cache_cap)
        return;                    /* already computed */

    /* recurrence coefficients (positive parts).  Negative coefficients
       handled by separate subtraction. */
    static const unsigned long long pos_coef[18] = {
        6ULL, 0ULL, 20ULL, 0ULL, 9ULL, 0ULL, 30ULL, 0ULL, 15ULL, 0ULL,
        12ULL, 0ULL, 12ULL, 0ULL, 1ULL, 0ULL, 3ULL, 0ULL
    };
    static const unsigned long long neg_coef[18] = {
        0ULL, 15ULL, 0ULL, 15ULL, 0ULL, 16ULL, 0ULL, 30ULL, 0ULL, 6ULL,
        0ULL, 18ULL, 0ULL, 3ULL, 0ULL, 3ULL, 0ULL, 1ULL
    };

    for (size_t k = cache_cap; k <= (size_t)n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (size_t j = 0; j < 18; ++j) {
            size_t idx = k - 1 - j;
            if (idx == 0)
                continue;          /* a(0) is unused */
            seq_t val = cache[idx];
            if (pos_coef[j])
                pos += val * (seq_t)pos_coef[j];
            if (neg_coef[j])
                neg += val * (seq_t)neg_coef[j];
        }
        cache[k] = pos - neg;      /* unsigned subtraction OK */
    }
    cache_cap = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    /* the specification guarantees n >= 1 */
    extend_cache(n);
    return cache[(size_t)n];
}
