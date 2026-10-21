/*
 * A090765  Let f(0) = 0, f(1) = 1 and for n > 1 let f(n) =
 *          (-1)*sum((-1)^(n+r)*f(r),r=0..n-2)/(n*(n-1)); sequence gives
 *          denominator of f(n).
 * https://oeis.org/A090765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n)=denominator of f(n) where f(0)=0, f(1)=1, for n>1
 *            f(n)=(-1)^(n+1) * sum_{r=0}^{n-2} (-1)^r f(r) / (n*(n-1)),
 *            a(0)=1, a(1)=1, a(2)=1,
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Fraction representation: sign (+1 or -1), numerator, denominator */
typedef struct {
    seq_t num;   /* absolute value, 0 allowed */
    seq_t den;   /* >=1, reduced */
    int   sign;  /* +1 or -1, zero is treated as +1 */
} Frac;

/* Euclidean gcd for seq_t (unsigned underlying type) */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Reduce a fraction to lowest terms */
static void reduce_frac(Frac *f)
{
    if (f->num == (seq_t)0) {
        f->den = (seq_t)1;
        f->sign = (int)1;
        return;
    }
    seq_t g = gcd_seq(f->num, f->den);
    if (g > (seq_t)1) {
        f->num = f->num / g;
        f->den = f->den / g;
    }
}

/* Add two fractions: res = a + b */
static Frac add_frac(const Frac *a, const Frac *b)
{
    Frac r;
    /* Zero handling */
    if (a->num == (seq_t)0) return *b;
    if (b->num == (seq_t)0) return *a;

    /* lcm of denominators: lcm = a.den / g * b.den */
    seq_t gden = gcd_seq(a->den, b->den);
    seq_t lcm  = a->den / gden * b->den;

    seq_t fac_a = lcm / a->den;
    seq_t fac_b = lcm / b->den;

    seq_t num_a = a->num * fac_a;
    seq_t num_b = b->num * fac_b;

    /* Combine signs */
    if (a->sign == b->sign) {
        r.num = num_a + num_b;
        r.sign = a->sign;
    } else {
        if (num_a >= num_b) {
            r.num = num_a - num_b;
            r.sign = a->sign;
        } else {
            r.num = num_b - num_a;
            r.sign = b->sign;
        }
    }
    r.den = lcm;
    if (r.num == (seq_t)0) r.sign = (int)1;
    reduce_frac(&r);
    return r;
}

/* ---- memoised cache of fractions ---- */
static Frac *f_cache = NULL;   /* f(n) fraction */
static Frac *T_cache = NULL;   /* T(n) = sum_{r=0}^n (-1)^r f(r) */
static size_t cache_cap = 0;
static idx_t  max_computed = 0;   /* highest n stored */

/* Ensure the cache can store up to index n */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
    Frac *tmp = (Frac *)realloc(f_cache, new_cap * sizeof(Frac));
    if (!tmp) exit(EXIT_FAILURE);
    f_cache = tmp;
    tmp = (Frac *)realloc(T_cache, new_cap * sizeof(Frac));
    if (!tmp) exit(EXIT_FAILURE);
    T_cache = tmp;
    cache_cap = new_cap;
    /* initialise base values if first time */
    if (max_computed == 0) {
        f_cache[0] = (Frac){0, (seq_t)1, (int)1};
        T_cache[0] = (Frac){0, (seq_t)1, (int)1};
    }
}

/* Precompute f(n) and T(n) up to n */
static void precompute_to(idx_t n)
{
    ensure_cache(n);
    while (max_computed < n) {
        idx_t cur = max_computed + 1;   /* next n to compute */

        if (cur == 1) {
            /* f(1)=1, T(1)= -1 */
            f_cache[1] = (Frac){1, (seq_t)1, (int)1};
            Frac neg = f_cache[1];
            neg.sign = -neg.sign;
            T_cache[1] = add_frac(&T_cache[0], &neg);
        } else {
            /* Compute f(cur) from T(cur-2) */
            const Frac *Tprev = &T_cache[cur-2];
            Frac fcur;
            fcur.num  = Tprev->num;
            /* denom = Tden * cur * (cur-1) */
            seq_t fac = (seq_t)cur * (seq_t)(cur - 1);
            fcur.den = Tprev->den * fac;
            /* sign = (-1)^(cur+1) * Tsign */
            fcur.sign = (cur % 2 == 0) ? -Tprev->sign : Tprev->sign;
            reduce_frac(&fcur);
            f_cache[cur] = fcur;

            /* Compute T(cur) = T(cur-1) + (-1)^cur * f(cur) */
            Frac term = fcur;
            term.sign = -term.sign;   /* multiply by (-1)^cur (which is -1 for odd cur) */
            if (cur % 2 == 0) term.sign = fcur.sign; /* even cur -> factor +1 */
            T_cache[cur] = add_frac(&T_cache[cur-1], &term);
        }
        max_computed = cur;
    }
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* f(0)=0 -> denominator 1 */
    precompute_to(n);
    return f_cache[n].den;
}
