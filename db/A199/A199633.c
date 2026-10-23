/*
 * A199633  Numbers having each digit once and whose 6th power has each digit
 *          six times.
 * https://oeis.org/A199633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 6, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search: enumerate 10‑digit pandigital numbers and select
   those whose 6th power has each decimal digit exactly six times. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Arbitrary precision helpers (implemented with 10⁹ base). */
#define BASE 1000000000U
typedef struct {
    uint32_t d[20];   /* little‑endian limbs; d[0] least significant */
    size_t   len;
} BigInt;

/* Multiply big integer by a small integer m (m < 1e10). */
static void big_mul_small(BigInt *b, uint64_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    while (carry) {
        b->d[b->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Count decimal digits of a big integer. */
static void digit_counts(const BigInt *b, unsigned char cnt[10])
{
    for (int i = 0; i < 10; ++i) cnt[i] = 0;

    /* Most significant limb – no leading zeros. */
    uint32_t ms = b->d[b->len - 1];
    if (ms == 0) {
        cnt[0] = 1;
    } else {
        while (ms > 0) {
            cnt[ms % 10]++;
            ms /= 10;
        }
    }

    /* Remaining limbs – always 9 digits with leading zeros. */
    for (long long i = (long long)b->len - 2; i >= 0; --i) {
        uint32_t v = b->d[i];
        for (int j = 0; j < 9; ++j) {
            cnt[v % 10]++;
            v /= 10;
        }
    }
}

/* Global storage for found solutions. */
static seq_t seq_sol[6];
static size_t seq_count = 0;

/* Append a new value if space is available. */
static void add_solution(seq_t val)
{
    if (seq_count < sizeof(seq_sol) / sizeof(seq_sol[0]))
        seq_sol[seq_count++] = val;
}

/* Threshold: smallest 10–digit number whose 6th power has 60 digits. */
static const uint64_t THRESH = 6800000000ULL;

/* Flag to stop enumeration once all solutions are found. */
static bool found_all = false;

/* Recursive generation of 10‑digit pandigitals. */
static void generate_rec(int pos, uint16_t used, uint64_t cur)
{
    if (found_all) return;
    if (pos == 10) {
        if (cur < THRESH) return;         /* 6th power too short */
        /* Build big integer from cur. */
        BigInt b = { .len = 0 };
        uint64_t t = cur;
        while (t) {
            b.d[b.len++] = (uint32_t)(t % BASE);
            t /= BASE;
        }

        /* Compute cur⁶ by successive multiplications. */
        for (int i = 0; i < 5; ++i)
            big_mul_small(&b, cur);

        /* Count digits. */
        unsigned char cnt[10];
        digit_counts(&b, cnt);
        for (int d = 0; d < 10; ++d)
            if (cnt[d] != 6) return;      /* not a solution */

        add_solution((seq_t)cur);
        if (seq_count == 6) found_all = true;
        return;
    }

    for (int d = 0; d <= 9; ++d) {
        if ((used >> d) & 1) continue;
        if (pos == 0 && d == 0) continue;   /* no leading zero */
        generate_rec(pos + 1, used | (1 << d), cur * 10 + d);
    }
}

/* Compute solutions once. */
static void init_solutions(void)
{
    generate_rec(0, 0, 0);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    static bool inited = false;
    if (!inited) { init_solutions(); inited = true; }

    if (n < 1) return (seq_t)0;
    size_t idx = (size_t)(n - 1);
    if (idx < seq_count) return seq_sol[idx];
    return (seq_t)0;
}
