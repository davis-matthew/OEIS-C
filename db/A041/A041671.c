/*
 * A041671  Denominators of continued fraction convergents to sqrt(354).
 * https://oeis.org/A041671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 516130*a(n-10) - a(n-20) for n>19 Initial values: a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=11, a(4)=27, a(5)=497, a(6)=1021,
 *            a(7)=2539, a(8)=11177, a(9)=13716, a(10)=504953, a(11)=518669,
 *            a(12)=2579629, a(13)=5677927, a(14)=13935483, a(15)=256516621,
 *            a(16)=526968725, a(17)=1310454071, a(18)=5768785009...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;   /* current allocated capacity (in seq_t elements) */
static size_t cache_len = 0;   /* number of valid elements stored */

/* Storage for the first 20 terms computed once */
static seq_t initial[20];
static bool initial_done = false;

/* Compute the first 20 denominators of convergents to sqrt(354) */
static void init_first20(void)
{
    if (initial_done)
        return;

    const unsigned long long S = 354ULL;
    unsigned long long m = 0ULL, d = 1ULL;
    unsigned long long a0 = (unsigned long long) sqrt((double) S);
    unsigned long long a = a0;

    unsigned long long p_prev2 = 0ULL, p_prev1 = 1ULL;
    unsigned long long q_prev2 = 1ULL, q_prev1 = 0ULL;
    unsigned long long p, q;

    for (unsigned idx = 0; idx < 20; ++idx) {
        /* Convergent recurrence */
        p = a * p_prev1 + p_prev2;
        q = a * q_prev1 + q_prev2;

        initial[idx] = (seq_t)q;   /* store denominator */

        /* shift for next step */
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;

        /* compute next partial quotient for sqrt(354) */
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (S - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        m = m_next;  d = d_next;  a = a_next;
    }

    initial_done = true;
}

/* Ensure cache has enough capacity for index n */
static void ensure_cache_size(idx_t n)
{
    size_t target = (size_t)n + 1;       /* we need indices 0..n inclusive */

    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 20;
        while (new_cap < target)
            new_cap <<= 1;                 /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = cache_len; i < target; ++i) {
        if (i < 20) {
            seq_cache[i] = initial[i];
        } else {
            seq_cache[i] = (seq_t)516130 * seq_cache[i - 10] - seq_cache[i - 20];
        }
    }

    if (target > cache_len)
        cache_len = target;
}

seq_t a(idx_t n)
{
    init_first20();                /* generate the 20 seed values once */
    ensure_cache_size(n);          /* compute up to requested index */
    return seq_cache[(size_t)n];
}
