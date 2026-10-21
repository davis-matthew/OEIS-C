/*
 * A078459  Numbers n such that in some base b <= log n the sum of the digits
 *          of n in base b equals b.
 * https://oeis.org/A078459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th natural number n₀ such that ∃ integer base b (2
 *            ≤ b ≤ ⌊log n₀⌋) with digit_sum(n₀, b) = b. Computes terms by
 *            brute‑force search; uses a pre‑computed table of ceil(eᵇ) values
 *            to enforce b ≤ log n₀
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static const idx_t ceil_e_pow[45] = {
    1ULL, 3ULL, 8ULL, 21ULL, 55ULL, 149ULL, 404ULL, 1097ULL, 2981ULL, 8104ULL,
    22027ULL, 59875ULL, 162755ULL, 442414ULL, 1202605ULL, 3269018ULL,
    8886111ULL, 24154953ULL, 65659970ULL, 178482301ULL, 485165196ULL,
    1318813165ULL, 3579279444ULL, 9694844857ULL, 26274450624ULL,
    71395365890ULL, 193883528488ULL, 526935257021ULL, 1430898529263ULL,
    3893381333665ULL, 10565282738360ULL, 28627034722736ULL,
    77565565047067ULL, 209679170628600ULL, 566796216416702ULL,
    1529633510254202ULL, 4136391285281525ULL, 11229718309377304ULL,
    30524123193065570ULL, 83254318445648219ULL, 226981130832428171ULL,
    616663161082442455ULL, 1676090379017171903ULL, 4563085135841641386ULL,
    12427473297140606995ULL
};

static seq_t *term_cache   = NULL;
static size_t cache_cap    = 0;
static size_t cache_size   = 0;
static idx_t  tested_upto  = 0;

static void ensure_capacity_seq(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static bool has_candidate(idx_t n)
{
    if (n < 2)
        return false;

    for (int base = 2; base <= 44; ++base)
    {
        if (n < ceil_e_pow[base])
            continue;   /* base > ln(n) */

        idx_t tmp  = n;
        idx_t sum  = 0;
        while (tmp > 0)
        {
            sum += tmp % (idx_t)base;
            if (sum > (idx_t)base)
                break;
            tmp /= (idx_t)base;
        }
        if (sum == (idx_t)base)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices < 1 */

    size_t needed = (size_t)n;

    while (cache_size < needed)
    {
        ++tested_upto;
        if (has_candidate(tested_upto))
        {
            ensure_capacity_seq(cache_size + 1);
            term_cache[cache_size++] = (seq_t)tested_upto;
        }
    }

    return term_cache[n - 1];
}
