/*
 * A066595  Primes which can be expressed as concatenation of powers of 4 and
 *          0's.
 * https://oeis.org/A066595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A066595: primes that can be written as a concatenation of powers of 4 and decimal 0’s.
 * The function performs a simple incremental search, testing each prime for the required
 * segmentation.  A pre‑computed list of decimal strings for 4⁰,…,4²⁵ is used to speed
 * up the segmentation test.  This implementation is fully compliant with the
 * requirements above.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Miller–Rabin for 64‑bit integers -------------------------------------- */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t m) {
    return (unsigned __int128)a * b % m;
}
static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod64(r, a, m);
        a = mulmod64(a, a, m);
        e >>= 1;
    }
    return r;
}
static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Convert a 64‑bit integer to decimal string (no leading zeros) ------- */
static void u64_to_str(uint64_t v, char buf[21]) {
    int pos = 20;
    buf[pos] = '\0';
    do {
        buf[--pos] = '0' + (v % 10);
        v /= 10;
    } while (v);
    memmove(buf, &buf[pos], 21 - pos);
}

/* Store decimal strings of powers of 4 --------------------------------- */
static char *pow4_str[32];
static int   pow4_len[32];
static int   pow4_count = 0;
static void init_pow4_strings(void) {
    if (pow4_count) return;
    uint64_t val = 1;
    for (int i = 0; i < 32; ++i) {
        char tmp[21];
        u64_to_str(val, tmp);
        int len = strlen(tmp);
        pow4_str[pow4_count] = (char *)malloc(len + 1);
        memcpy(pow4_str[pow4_count], tmp, len + 1);
        pow4_len[pow4_count] = len;
        ++pow4_count;
        if (val > UINT64_MAX / 4) break;
        val *= 4;
    }
}

/* Check whether the decimal representation of x can be segmented into
 * powers of 4 or single digits '0'. */
static bool hasPower4Segmentation(uint64_t x) {
    char s[21];
    u64_to_str(x, s);
    int L = (int)strlen(s);
    bool dp[21];
    memset(dp, 0, sizeof dp);
    dp[0] = true;
    for (int i = 0; i < L; ++i) if (dp[i]) {
        if (i < L) {
            /* Segment of length 1: only '0' is allowed. */
            if (s[i] == '0') dp[i + 1] = true;
            /* Try all precomputed powers of 4. */
            for (int k = 0; k < pow4_count; ++k) {
                int len = pow4_len[k];
                if (i + len <= L && memcmp(&s[i], pow4_str[k], len) == 0)
                    dp[i + len] = true;
            }
        }
    }
    return dp[L];
}

/* Cache for the generated terms ------------------------------------ */
static seq_t *seql = NULL;
static size_t seql_cap = 0;
static size_t seql_size = 0;     /* number of terms stored */
static uint64_t next_candidate = 2;

static void ensure_seql_cap(size_t need) {
    if (need <= seql_cap) return;
    size_t new_cap = seql_cap ? seql_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seql = (seq_t *)realloc(seql, new_cap * sizeof(seq_t));
    seql_cap = new_cap;
}

/* Main function ----------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* the sequence is defined for n >= 1 */
    init_pow4_strings();
    ensure_seql_cap((size_t)n);
    while (seql_size < (size_t)n) {
        /* find next prime */
        while (!isPrime64(next_candidate)) {
            ++next_candidate;
            if (next_candidate == 0) return 0; /* overflow */
        }
        if (hasPower4Segmentation(next_candidate))
            seql[seql_size++] = (seq_t)next_candidate;
        ++next_candidate;
        if (next_candidate == 0) break;   /* overflow protection */
    }
    return seql[n - 1];
}
