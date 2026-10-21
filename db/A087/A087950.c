/*
 * A087950  Numerators k for which the partial quotients of the k-CF of
 *          sqrt(2) are periodic, where a k-CF is defined as the continued
 *          fraction representation having k as the constant numerator: x =
 *          q_0 + k/(q_1 + k/(q_2 + k/(q_3 +...))).
 * https://oeis.org/A087950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function generates A087950 (OEIS sequence of numerators k
 *            for which the k‑continued fraction of sqrt(2) is periodic)
 * Range:     u8 n <= 58, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   abs128, hash_state, leq128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* headers that are available in both C11 and C++17 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/abs128.h"
#include "../utils/hash_state.h"
#include "../utils/leq128.h"

/* gcd for signed __int128 */
static inline __int128 gcd128(__int128 a, __int128 b)
{
    a = abs128(a);
    b = abs128(b);
    while (b != 0) {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* structure for a state (p+q*sqrt2)/r */
typedef struct {
    __int128 a;
    __int128 b;
    __int128 c;
} state_t;

/* size of the hash table for cycle detection */
#define HASH_SIZE 8192
#define HASH_MASK (HASH_SIZE-1)

/* check if k generates a periodic k‑CF of sqrt(2) */
static bool is_periodic(uint64_t k)
{
    /* hash table entry */
    struct entry {
        bool used;
        state_t st;
    } table[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i) table[i].used = false;

    /* initial state: sqrt(2) = (0+1*sqrt2)/1 */
    state_t cur = {0, 1, 1};

    /* constant sqrt(2) */
    static const long double SQRT2 = sqrt((long double)2.0L);

    const int MAX_STEPS = 2000; /* sufficient for our purposes */

    for (int step = 0; step < MAX_STEPS; ++step) {
        /* hash and check for repetition */
        uint64_t h = hash_state(cur.a, cur.b, cur.c);
        uint64_t idx = h & HASH_MASK;
        for (;;) {
            if (!table[idx].used) {
                table[idx].used = true;
                table[idx].st = cur;
                break;
            }
            if (table[idx].st.a == cur.a &&
                table[idx].st.b == cur.b &&
                table[idx].st.c == cur.c) {
                return true; /* cycle detected */
            }
            idx = (idx + 1) & HASH_MASK;
        }

        /* compute floor(a) for current r = (a + b*sqrt2)/c */
        long double approx = ((long double)cur.a / (long double)cur.c)
                           + ((long double)cur.b * SQRT2) / (long double)cur.c;
        idx_t a_guess = (idx_t)floor(approx);

        /* adjust to exact floor using arithmetic tests */
        while (!leq128(cur.a, cur.b, cur.c, (__int128)a_guess))
            --a_guess;
        while (leq128(cur.a, cur.b, cur.c, (__int128)(a_guess + 1)))
            ++a_guess;

        /* next state by the recurrence */
        __int128 a = cur.a, b = cur.b, c = cur.c;
        __int128 A = (__int128)a_guess;

        __int128 D = a - A * c;                        /* fractional numerator */
        __int128 E = (__int128)k * c * D;              /* new a */
        __int128 F = -(__int128)k * c * b;             /* new b */
        __int128 G = D * D - (__int128)2 * b * b;      /* new c */

        if (G < 0) { G = -G; E = -E; F = -F; }

        /* reduce by gcd */
        __int128 g = gcd128(abs128(E), abs128(F));
        g = gcd128(g, abs128(G));
        if (g > 1) {
            E /= g;
            F /= g;
            G /= g;
        }

        cur.a = E;
        cur.b = F;
        cur.c = G;
    }
    return false; /* no period found within limit */
}

/* dynamic cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_checked_k = 1;

static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* out of memory – should not happen */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* the required function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    while (cache_size < (size_t)n) {
        if (is_periodic((uint64_t)last_checked_k)) {
            cache[cache_size++] = (seq_t)last_checked_k;
        }
        ++last_checked_k;
    }
    return cache[n - 1];
}
