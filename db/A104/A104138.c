/*
 * A104138  Smallest prime followed by n or more composites.
 * https://oeis.org/A104138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime that is followed by at least n
 *            composite numbers. For n = 0, the definition is a(0) = 2. For n
 *            > 0, a(n) is the first prime p for which p < q = next prime and
 *            (q - p - 1) ≥ n
 * Range:     u8 n <= 13, u16 n <= 71, u32 n <= 335, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 973 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the first prime followed by a gap of at least n + 1, i.e. the
 * first record ("maximal") prime gap of size > n. Below 2^32 the records
 * are found here, with a segmented sieve of Eratosthenes; the records above
 * that (gaps 354 and up, primes past 4.3e9, a(1000) = 1693182318746371) take
 * a sieve far beyond reach and are the proven values of the record-gap
 * searches (T. Nicely, T. Oliveira e Silva et al.), from A002386 (the primes)
 * and A005250 (the gaps): {high 64 bits, low 64 bits, gap}. (The original
 * tested every number by trial division and timed out near n = 340.) */

#include <stdint.h>
#include <stdlib.h>

static const struct { uint64_t hi, lo; unsigned gap; } REC[] = {
    {0ull, 4302407359ull, 354},
    {0ull, 10726904659ull, 382},
    {0ull, 20678048297ull, 384},
    {0ull, 22367084959ull, 394},
    {0ull, 25056082087ull, 456},
    {0ull, 42652618343ull, 464},
    {0ull, 127976334671ull, 468},
    {0ull, 182226896239ull, 474},
    {0ull, 241160624143ull, 486},
    {0ull, 297501075799ull, 490},
    {0ull, 303371455241ull, 500},
    {0ull, 304599508537ull, 514},
    {0ull, 416608695821ull, 516},
    {0ull, 461690510011ull, 532},
    {0ull, 614487453523ull, 534},
    {0ull, 738832927927ull, 540},
    {0ull, 1346294310749ull, 582},
    {0ull, 1408695493609ull, 588},
    {0ull, 1968188556461ull, 602},
    {0ull, 2614941710599ull, 652},
    {0ull, 7177162611713ull, 674},
    {0ull, 13829048559701ull, 716},
    {0ull, 19581334192423ull, 766},
    {0ull, 42842283925351ull, 778},
    {0ull, 90874329411493ull, 804},
    {0ull, 171231342420521ull, 806},
    {0ull, 218209405436543ull, 906},
    {0ull, 1189459969825483ull, 916},
    {0ull, 1686994940955803ull, 924},
    {0ull, 1693182318746371ull, 1132},
    {0ull, 43841547845541059ull, 1184},
    {0ull, 55350776431903243ull, 1198},
    {0ull, 80873624627234849ull, 1220},
    {0ull, 203986478517455989ull, 1224},
    {0ull, 218034721194214273ull, 1248},
    {0ull, 305405826521087869ull, 1272},
    {0ull, 352521223451364323ull, 1328},
    {0ull, 401429925999153707ull, 1356},
    {0ull, 418032645936712127ull, 1370},
    {0ull, 804212830686677669ull, 1442},
    {0ull, 1425172824437699411ull, 1476},
    {0ull, 5733241593241196731ull, 1488},
    {0ull, 6787988999657777797ull, 1510},
    {0ull, 15570628755536096243ull, 1526},
    {0ull, 17678654157568189057ull, 1530},
    {0ull, 18361375334787046697ull, 1550},
    {1ull, 23313872551146615ull, 1552},
    {1ull, 124929358342278483ull, 1572},
    {1ull, 2287002436851891247ull, 1676},
    {3ull, 12728578062105528059ull, 1724},
    {5ull, 9178599627815550989ull, 1854},
};
#define NREC (sizeof REC / sizeof REC[0])
#define LIMIT ((uint64_t)1 << 32)              /* the sieve stops here */

static uint64_t rp[64];                        /* records found by the sieve */
static unsigned rg[64];
static int nr = 0;
static uint64_t sieved = 0, last_prime = 0;    /* [0, sieved) done */

static void sieve_more(void)
{
    static uint32_t *base = NULL;              /* primes up to 2^16 */
    static size_t nb = 0;
    if (!base) {
        unsigned char c[65537] = {0};
        base = (uint32_t *)malloc(7000 * sizeof *base);
        if (!base) abort();
        for (uint32_t i = 2; i <= 65536; ++i) {
            if (c[i]) continue;
            base[nb++] = i;
            for (uint32_t j = i * i; j <= 65536; j += i) c[j] = 1;
        }
    }
    const uint64_t W = 1 << 22;
    static unsigned char *seg = NULL;
    if (!seg && !(seg = (unsigned char *)malloc(W))) abort();
    uint64_t lo = sieved, hi = lo + W < LIMIT ? lo + W : LIMIT;
    for (uint64_t i = 0; i < hi - lo; ++i) seg[i] = 1;
    for (size_t j = 0; j < nb; ++j) {
        uint64_t p = base[j];
        if (p * p >= hi) break;
        uint64_t m = p * p > lo ? p * p : (lo + p - 1) / p * p;
        for (; m < hi; m += p) seg[m - lo] = 0;
    }
    for (uint64_t x = lo < 2 ? 2 : lo; x < hi; ++x) {
        if (!seg[x - lo]) continue;
        if (last_prime) {
            unsigned g = (unsigned)(x - last_prime);
            if (nr == 0 || g > rg[nr - 1]) { rp[nr] = last_prime; rg[nr++] = g; }
        }
        last_prime = x;
    }
    sieved = hi;
}

seq_t a(idx_t n)
{
    for (;;) {
        for (int i = 0; i < nr; ++i)
            if (rg[i] >= n + 1) return (seq_t)rp[i];
        if (sieved >= LIMIT) break;
        sieve_more();
    }
    for (size_t i = 0; i < NREC; ++i)
        if (REC[i].gap >= n + 1) {
            seq_t hi = (seq_t)REC[i].hi, lo = (seq_t)REC[i].lo;
            return sizeof(seq_t) > 8 ? ((hi << 32) << 32) + lo : lo;
        }
    return (seq_t)0;                           /* past the known records */
}
