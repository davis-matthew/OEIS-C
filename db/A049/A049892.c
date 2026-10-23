/*
 * A049892  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 3.
 * https://oeis.org/A049892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=3. For n≥4, let p be the unique integer
 *            with 2^p < n-1 ≤ 2^(p+1). Then a(n) = (a(1)+…+a(n-1)) –
 *            a(2*n-3-2^(p+1))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(idx_t n) may be called in arbitrary order; it memoises
   previously computed values in static arrays.  */
/* The implementation uses a simple dynamic array growth
   strategy and a helper function to compute floor(log2).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: floor(log2(x)) for x>0.  Works for 64‑bit unsigned.  */
static inline unsigned int floor_log2_u64(uint64_t x)
{
    /* x > 0 when called.  */
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(x);
#else
    unsigned int r = 0;
    while (x >>= 1) ++r;
    return r;
#endif
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;          /* a[1] … a[maxStored]  */
    static seq_t *Pref = NULL;       /* prefix sums: Pref[i] = Σ_{k=1}^i a[k]  */
    static size_t capacity = 0;      /* allocated size (>= maxStored)  */
    static size_t maxStored = 0;     /* highest index computed so far  */

    if (n < 1) return (seq_t)0;      /* out of range – not used in tests */

    /* Grow the arrays if necessary (size needed ≥ n).  */
    if ((size_t)n > capacity)
    {
        size_t newCap = capacity ? capacity * 2 : 4;
        while (newCap < (size_t)n) newCap <<= 1;

        seq_t *newA    = (seq_t*)realloc(A,    (newCap + 1) * sizeof(seq_t));
        seq_t *newPref = (seq_t*)realloc(Pref, (newCap + 1) * sizeof(seq_t));
        if (!newA || !newPref) abort(); /* memory exhaustion */

        /* If this is the first allocation, initialise Pref[0] */
        if (capacity == 0) newPref[0] = (seq_t)0;

        A    = newA;
        Pref = newPref;
        capacity = newCap;
    }

    /* Compute values only up to n.  */
    for (size_t i = maxStored + 1; i <= (size_t)n; ++i)
    {
        if (i <= 3)
        {
            if (i == 1 || i == 2) A[i] = (seq_t)1;
            else                  A[i] = (seq_t)3;
        }
        else
        {
            idx_t x = (idx_t)i - 2;                                   /* x > 0 */
            unsigned int p = floor_log2_u64(x);                       /* 2^p < x ≤ 2^(p+1) */
            idx_t power = ((idx_t)1) << (p + 1);                       /* 2^(p+1)   */
            idx_t m = 2 * (idx_t)i - 3 - power;                       /* index to subtract */
            A[i] = Pref[i-1] - A[m];
        }
        Pref[i] = Pref[i-1] + A[i];                                   /* maintain prefix sum */
    }

    maxStored = (size_t)n;   /* all indices up to n are now cached */

    return A[n];
}
