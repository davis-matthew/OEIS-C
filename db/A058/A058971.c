/*
 * A058971  For a rational number p/q let f(p/q) = sum of divisors of p+q
 *          divided by number of divisors of p+q; a(n) is obtained by
 *          iterating f, starting at n/1, until an integer is reached, or if
 *          no integer is ever reached then a(n) = 0.
 * https://oeis.org/A058971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 322, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A058971 – iterate f(p/q) = sigma(p+q)/tau(p+q) until an integer is reached.
   If the process enters a cycle, the value 0 is returned.  */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
/*  Helper routines: Euclidean GCD, integer square‑root and sigma/tau.        */
/* ------------------------------------------------------------------------- */

static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2)
        return n;
    uint64_t x = n;
    uint64_t y = (x + n / x) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/*  Return sigma(m) (sum of divisors) and store tau(m) (number of divisors). */
static inline uint64_t sigma_tau(uint64_t m, uint64_t *tau)
{
    uint64_t sum = 0;
    uint64_t cnt = 0;
    uint64_t r   = isqrt_u64(m);
    for (uint64_t i = 1; i <= r; ++i) {
        if (m % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = m / i;
            sum += d1;
            ++cnt;
            if (d1 != d2) {
                sum += d2;
                ++cnt;
            }
        }
    }
    *tau = cnt;
    return sum;
}

/* ------------------------------------------------------------------------- */
/*  Main sequence function.                                                  */
/* ------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Working values – use 64‑bit to avoid overflow in intermediate steps. */
    uint64_t p = (uint64_t)n;   /* rational numerator   */
    uint64_t q = 1ULL;          /* rational denominator */

    /* Simple dynamic array for cycle detection (pair of (p,q)). */
    struct Pair { uint64_t p, q; };
    size_t cap  = 8;
    size_t sz   = 0;
    struct Pair *arr = (struct Pair *)malloc(cap * sizeof(struct Pair));

    for (;;) {
        /* Compute sigma(p+q)/tau(p+q). */
        uint64_t sum   = p + q;
        uint64_t tau;
        uint64_t sig   = sigma_tau(sum, &tau);

        uint64_t g     = gcd_u64(sig, tau);
        uint64_t newP  = sig / g;
        uint64_t newQ  = tau / g;

        /* Termination: integer reached. */
        if (newQ == 1ULL) {
            free(arr);
            return (seq_t)newP;            /* cast to seq_t – fits if it is small. */
        }

        /* Cycle detection. */
        for (size_t i = 0; i < sz; ++i)
            if (arr[i].p == newP && arr[i].q == newQ) {
                free(arr);
                return 0;                 /* cycle detected – return 0. */
            }

        /* Store current state and proceed. */
        if (sz == cap) {
            cap <<= 1;
            arr = (struct Pair *)realloc(arr, cap * sizeof(struct Pair));
        }
        arr[sz].p = newP;
        arr[sz].q = newQ;
        ++sz;

        p = newP;
        q = newQ;
    }
}
