/*
 * A007666  a(n) = smallest number k such that k^n is the sum of n positive
 *          n-th powers, or 0 if no solution exists.
 * https://oeis.org/A007666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, nice, more
 * Formula:   search over k = 2, 3, ...; k^n is tested as a sum of n positive
 *            n-th powers. n-2 terms are chosen in nonincreasing order by
 *            depth-first search, and the last two come from a hash table of
 *            sums of two n-th powers. a(1) = 1. Returns 0 if no k is found
 *            within the search budget or before k^n overflows...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *H;
static size_t hcap, hcnt;
static uint64_t *pw;
static unsigned long long budget;

static size_t hslot(uint64_t v) {
    uint64_t h = v * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(hcap - 1));
}

static void hput_raw(uint64_t v) {
    size_t i = hslot(v);
    while (H[i] != 0) {
        if (H[i] == v) return;
        i = (i + 1) & (hcap - 1);
    }
    H[i] = v;
    hcnt++;
}

static void hinsert(uint64_t v) {
    if ((hcnt + 1) * 2 > hcap) {
        uint64_t *old = H;
        size_t oc = hcap, i;
        hcap = oc ? oc * 2 : 1024;
        H = (uint64_t *)calloc(hcap, sizeof(uint64_t));
        hcnt = 0;
        for (i = 0; i < oc; i++) if (old[i]) hput_raw(old[i]);
        free(old);
    }
    hput_raw(v);
}

static bool hfind(uint64_t v) {
    size_t i;
    if (hcap == 0 || v == 0) return false;
    i = hslot(v);
    while (H[i] != 0) {
        if (H[i] == v) return true;
        i = (i + 1) & (hcap - 1);
    }
    return false;
}

static bool dfs(uint64_t rem, long cnt, uint64_t maxx) {
    uint64_t lo, hi, lim, need, x;
    if (cnt == 0) return hfind(rem);
    if (rem < (uint64_t)cnt + 2) return false;
    lim = rem - (uint64_t)(cnt + 1);
    if (pw[1] > lim) return false;
    lo = 1; hi = maxx;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (pw[mid] <= lim) lo = mid; else hi = mid - 1;
    }
    need = rem / (uint64_t)(cnt + 2) + (rem % (uint64_t)(cnt + 2) != 0);
    for (x = lo; x >= 1 && pw[x] >= need; x--) {
        if (++budget > 300000000ULL) return false;
        if (dfs(rem - pw[x], cnt - 1, x)) return true;
    }
    return false;
}

static uint64_t solve(uint64_t n) {
    uint64_t k, result = 0;
    size_t pcap = 1024;
    if (n == 1) return 1;
    H = NULL; hcap = 0; hcnt = 0; budget = 0;
    pw = (uint64_t *)malloc(pcap * sizeof(uint64_t));
    pw[0] = 0;
    pw[1] = 1;
    for (k = 2;; k++) {
        uint64_t pk = 1, e, a2;
        bool ovf = false;
        for (e = 0; e < n; e++) {
            if (pk > UINT64_MAX / k) { ovf = true; break; }
            pk *= k;
        }
        if (ovf) break;
        for (a2 = 1; a2 <= k - 1; a2++) {
            if (pw[a2] <= UINT64_MAX - pw[k - 1]) hinsert(pw[a2] + pw[k - 1]);
            budget++;
        }
        if (dfs(pk, (long)n - 2, k - 1)) { result = k; break; }
        if (budget > 300000000ULL) break;
        if ((size_t)k + 1 >= pcap) {
            pcap *= 2;
            pw = (uint64_t *)realloc(pw, pcap * sizeof(uint64_t));
        }
        pw[k] = pk;
    }
    free(pw); pw = NULL;
    free(H); H = NULL; hcap = 0; hcnt = 0;
    return result;
}

seq_t a(idx_t n) {
    static uint64_t memo[64];
    static bool have[64];
    uint64_t r;
    if (n < 64 && have[n]) return (seq_t)memo[n];
    r = solve((uint64_t)n);
    if (n < 64) { memo[n] = r; have[n] = true; }
    return (seq_t)r;
}
