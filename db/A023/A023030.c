/*
 * A023030  Number of partitions of n into 10 unordered relatively prime
 *          parts.
 * https://oeis.org/A023030
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) · p10(n/d) (OEIS A023030) * where p10(m) is
 *            the number of partitions of m into exactly 10 parts
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Utility: Möbius function via smallest prime factor table           */
/* ------------------------------------------------------------------ */
static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i   */
static size_t spf_len = 0;          /* length of spf array (last index)      */

static void ensure_spf(size_t N)
{
    if (spf_len >= N) return;

    free(spf);
    spf = (idx_t *)calloc(N + 1, sizeof(idx_t));
    if (!spf) return;                 /* allocation failure */

    spf[0] = 0;
    if (N >= 1) spf[1] = 1;

    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (idx_t)i;
            if (i <= N / i) {
                for (size_t j = i * i; j <= N; j += i) {
                    if (spf[j] == 0) spf[j] = (idx_t)i;
                }
            }
        }
    }
    spf_len = N;
}

/* Möbius value using pre‑computed spf */
static int mobius_from_spf(idx_t x)
{
    if (x == 1) return 1;
    int cnt = 0;
    while (x > 1) {
        idx_t p = spf[x];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            exp++;
            if (exp > 1) return 0;  /* squared prime factor */
        }
        cnt++;
    }
    return (cnt & 1) ? -1 : 1;
}

/* ------------------------------------------------------------------ */
/* Partition DP: partitions of s using parts ≤10 (equivalent to at most
 * 10 parts)                                                  */
/* ------------------------------------------------------------------ */
static seq_t *dp_atmost = NULL;
static size_t dp_len = 0;

static void ensure_dp(size_t N)
{
    if (dp_len >= N) return;

    free(dp_atmost);
    dp_atmost = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp_atmost) return;          /* allocation failure */

    dp_atmost[0] = 1;                /* empty partition */

    for (idx_t part = 1; part <= 10; ++part) {
        for (size_t s = part; s <= N; ++s) {
            dp_atmost[s] = dp_atmost[s] + dp_atmost[s - part];
        }
    }
    dp_len = N;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 10) return 0;            /* offset */

    size_t N = (size_t)n;
    ensure_spf(N);
    ensure_dp(N);

    long long acc = 0;               /* accumulate with sign */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            int mu1 = mobius_from_spf(d);
            idx_t m1 = n / d;
            if (m1 >= 10) {
                seq_t p10 = dp_atmost[m1 - 10];
                acc += (long long)mu1 * (long long)p10;
            }

            idx_t d2 = n / d;
            if (d2 != d) {
                int mu2 = mobius_from_spf(d2);
                idx_t m2 = n / d2;
                if (m2 >= 10) {
                    seq_t p10 = dp_atmost[m2 - 10];
                    acc += (long long)mu2 * (long long)p10;
                }
            }
        }
    }

    return (seq_t)acc;
}
