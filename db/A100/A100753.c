/*
 * A100753  Numbers which can be obtained as the product of one or more of
 *          their digits (used with multiplicity).
 * https://oeis.org/A100753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the sequence by testing each integer for membership
 * in the set of positive integers that can be expressed as a product of
 * one or more of their decimal digits (used with multiplicity). */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* helper data: prime exponents for digits 2..9 */
static const int twoExp[10] = {0,0,1,0,2,0,1,0,3,0};
static const int threeExp[10] = {0,0,0,1,0,0,1,0,0,2};

/* Test whether a number satisfies the property. */
static bool is_special(seq_t value)
{
    if (value == (seq_t)1) return true;           /* 1 is always valid */

    /* ---- 1. digit set ---- */
    uint16_t mask = 0;
    seq_t tmp = value;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        mask |= 1 << d;
        tmp /= (seq_t)10;
    }

    /* ---- 2. factorisation (only 2,3,5,7 allowed) ---- */
    int e2 = 0, e3 = 0, e5 = 0, e7 = 0;
    while ((value % (seq_t)2) == (seq_t)0) { e2++; value /= (seq_t)2; }
    while ((value % (seq_t)3) == (seq_t)0) { e3++; value /= (seq_t)3; }
    while ((value % (seq_t)5) == (seq_t)0) { e5++; value /= (seq_t)5; }
    while ((value % (seq_t)7) == (seq_t)0) { e7++; value /= (seq_t)7; }
    if (value != (seq_t)1) return false;          /* contains other prime */

    /* ---- 3. check availability of 5 and 7 ---- */
    if (e5 > 0 && !(mask & (1 << 5))) return false;
    if (e7 > 0 && !(mask & (1 << 7))) return false;

    /* ---- 4. if only 5/7 factors remain, we are done ---- */
    if (e2 == 0 && e3 == 0) return true;

    /* ---- 5. collect vectors of (e2,e3) for digits that can contribute them ---- */
    struct {int e2; int e3;} vec[8];
    int vecCnt = 0;
    for (int d = 2; d <= 9; ++d) {
        if (mask & (1 << d)) {
            int a = twoExp[d];
            int b = threeExp[d];
            if (a || b) {
                vec[vecCnt].e2 = a;
                vec[vecCnt].e3 = b;
                ++vecCnt;
            }
        }
    }
    if (vecCnt == 0) return false;   /* no digit can provide needed 2 or 3 factor */

    /* ---- 6. BFS over exponent space ---- */
    /* Exponents are tiny (≤63 for 2, ≤38 for 3 in 64‑bit range) */
    static unsigned char visited[70][70];
    for (int i = 0; i <= e2; ++i)
        memset(visited[i], 0, e3 + 1);

    int q2[70*70], q3[70*70];
    int head = 0, tail = 0;
    q2[tail] = 0; q3[tail] = 0; visited[0][0] = 1; ++tail;

    while (head < tail) {
        int cur2 = q2[head];
        int cur3 = q3[head];
        ++head;

        if (cur2 == e2 && cur3 == e3) return true;

        for (int i = 0; i < vecCnt; ++i) {
            int ne2 = cur2 + vec[i].e2;
            int ne3 = cur3 + vec[i].e3;
            if (ne2 <= e2 && ne3 <= e3 && !visited[ne2][ne3]) {
                visited[ne2][ne3] = 1;
                q2[tail] = ne2;
                q3[tail] = ne3;
                ++tail;
            }
        }
    }
    return false;
}

/* main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of discovered terms, 1-indexed */
    static seq_t *cache = NULL;          /* cache[0] unused */
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 1;     /* next integer to test */

    if (n < 1) return (seq_t)0;          /* no definition for n<1 */

    if (n > cache_len) {
        /* ensure capacity */
        if (cache_cap < n) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < n) new_cap = n;
            seq_t *new_cache = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;   /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        seq_t cand = next_candidate;
        while (cache_len < n) {
            if (is_special(cand)) {
                ++cache_len;
                cache[cache_len] = cand;
            }
            ++cand;
        }
        next_candidate = cand;
    }
    return cache[n];
}
