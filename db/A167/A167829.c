/*
 * A167829  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*(a(n-1)+...+a(n-14)) - 741*a(n-15) for n≥16 with
 *            a(0)=1, a(1)=40, a(2)=1560, a(3)=60840, a(4)=2372760,
 *            a(5)=92537640, a(6)=3608967960, a(7)=140749750440,
 *            a(8)=5489240267160, a(9)=214080370419240,
 *            a(10)=8349134446350360, a(11)=325616243407664040,
 *            a(12)=12699033492898897560, a(13)=4952623...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* parse an arbitrary-length decimal string into seq_t */
static inline seq_t parse_decimal(const char *s)
{
    seq_t v = 0;
    for (; *s; ++s) {
        v = v * (seq_t)10 + (seq_t)(*s - '0');
    }
    return v;
}

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* initialise cache with the first 15 explicit terms */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* out of memory – caller must never see this */

        cache[0] = parse_decimal("1");
        cache[1] = parse_decimal("40");
        cache[2] = parse_decimal("1560");
        cache[3] = parse_decimal("60840");
        cache[4] = parse_decimal("2372760");
        cache[5] = parse_decimal("92537640");
        cache[6] = parse_decimal("3608967960");
        cache[7] = parse_decimal("140749750440");
        cache[8] = parse_decimal("5489240267160");
        cache[9] = parse_decimal("214080370419240");
        cache[10] = parse_decimal("8349134446350360");
        cache[11] = parse_decimal("325616243407664040");
        cache[12] = parse_decimal("12699033492898897560");
        cache[13] = parse_decimal("495262306223057004840");
        cache[14] = parse_decimal("19315229942699223188760");

        /* compute a(15) from the recurrence with the non‑zero term 1 */
        seq_t sum = 0;
        for (int k = 1; k <= 14; ++k)
            sum += cache[(size_t)k];

        seq_t term = sum * (seq_t)38 - cache[0] * (seq_t)741 + (seq_t)1;
        cache[15] = term;

        cache_size = 16;   /* indices 0..15 inclusive are filled */
    }

    /* if requested term is already cached, return it */
    if ((idx_t)n < (idx_t)cache_size)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 14; ++k)
            sum += cache[(size_t)(i - k)];

        seq_t next = sum * (seq_t)38 - cache[(size_t)(i - 15)] * (seq_t)741;

        /* grow cache if necessary */
        if (cache_size + 1 > cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return 0;   /* out of memory – caller must never see this */
        }

        cache[cache_size++] = next;
    }

    return cache[(size_t)n];
}
