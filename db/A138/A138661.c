/*
 * A138661  Expansion of a level 11 weight 7 multiplicative modular form in
 *          powers of q.
 * https://oeis.org/A138661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 30, i32 n <= 960, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A138661 – multiplicative modular form coefficients
 *
 *  a(n) is multiplicative with
 *      a(1) = 1
 *      a(11^e) = (-1331)^e
 *  For a prime p ≠ 11 :
 *      If p ≡ 2,6,7,8,10 (mod 11):
 *          a(p^e) = p^(3e) * (1+(-1)^e)/2
 *          i.e. 0 for odd e, p^(3e) for even e.
 *      If p ≡ 1,3,4,5,9 (mod 11):
 *          Let y be an integer with 4p = y² + 11x²  (x exists by the
 *          definition).  Then
 *              a(p)   = y⁶ - 6 p y⁴ + 9 p² y² - 2 p³
 *              a(p^e) = a(p) * a(p^(e-1)) - p⁶ * a(p^(e-2)),  e ≥ 2
 *
 *  Zero pattern: if n ≡ 2 (mod 4) or  n ≡ 2,6,7,8,10 (mod 11)
 *  the term is 0.  This follows from a(p^1)=0 for any prime p in the
 *  second class or ∈ {2} from the modulo‑4 condition.
 *
 *  The function below implements this definition by prime
 *  factorisation.  It never hard‑codes terms and works for all n ≥ 1.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* --- helpers --------------------------------------------------- */

static inline uint64_t int_sqrt(uint64_t v)
{
    uint64_t r = (uint64_t)sqrt((long double)v);
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

static bool find_y_for_prime(uint64_t p, uint64_t *yp)
{
    /* Find integers x,y with 4p = y² + 11x².  Brute‑force over x. */
    uint64_t limit = int_sqrt(4*p / 11);
    for (uint64_t x = 0; x <= limit; ++x) {
        uint64_t rem = 4 * p - 11 * x * x;
        uint64_t y = int_sqrt(rem);
        if (y * y == rem) {
            *yp = y;
            return true;
        }
    }
    return false;   /* should never happen for the primes in question */
}

static inline seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* value for a(p^e) where p is a prime not equal to 11
 * e ≥ 1
 */
static seq_t a_prime_power(uint64_t p, int e)
{
    unsigned int mod = (unsigned int)(p % 11);
    if (p == 11) { /* handled separately */
        return 1; /* placeholder */
    }
    /* zero class primes – a(p^e) is zero for odd exponent,
     * otherwise p^(3e)
     */
    if (mod == 2 || mod == 6 || mod == 7 || mod == 8 || mod == 10) {
        if (e & 1) return 0;
        return pow_seq((seq_t)p, 3 * (unsigned int)e);
    }
    /* recurrence class primes */
    uint64_t y;
    find_y_for_prime(p, &y);
    seq_t y_seq = (seq_t)y;
    seq_t y2    = y_seq * y_seq;          /* y² */
    seq_t y4    = y2 * y2;                /* y⁴ */
    seq_t y6    = y4 * y2;                /* y⁶ */
    seq_t p_seq = (seq_t)p;
    seq_t p2    = p_seq * p_seq;          /* p² */
    seq_t p3    = p2 * p_seq;             /* p³ */
    seq_t a_p   = y6 - (seq_t)6 * p_seq * y4
                        + (seq_t)9 * p2 * y2
                        - (seq_t)2 * p3;   /* a(p) */

    if (e == 1) return a_p;
    seq_t p6 = pow_seq(p_seq, 6);
    seq_t prev2 = (seq_t)1;   /* a(p⁰) */
    seq_t prev1 = a_p;        /* a(p¹) */
    seq_t cur;
    for (int i = 2; i <= e; ++i) {
        cur = a_p * prev1 - p6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1)=1, and n<1 is irrelevant */

    idx_t tmp = n;
    seq_t result = (seq_t)1;

    /* trial division – small enough for n ≤ 10 000 */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            seq_t val;
            if (p == 11) {
                val = pow_seq((seq_t)-1331, (unsigned int)e);
            } else {
                val = a_prime_power(p, e);
            }

            if (val == 0) return (seq_t)0;
            result *= val;
        }
    }

    if (tmp > 1) {   /* remaining prime factor */
        uint64_t p = tmp;
        seq_t val;
        if (p == 11) {
            val = pow_seq((seq_t)-1331, 1);
        } else {
            val = a_prime_power(p, 1);
        }
        if (val == 0) return (seq_t)0;
        result *= val;
    }

    return result;
}
