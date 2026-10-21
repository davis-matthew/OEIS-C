/*
 * A209739  Number of n X 3 0..5 arrays with every 2 X 2 subblock containing
 *          exactly one value repeat, and new values 0..5 introduced in row
 *          major order.
 * https://oeis.org/A209739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 100*a(n-1) -2365*a(n-2)+6020*a(n-3)+291532*a(n-4)
 *            -2681320*a(n-5)+4701820*a(n-6)+23602304*a(n-7)-82586672*a(n-8)
 *            +27026368*a(n-9)+113762816*a(n-10)-78159872*a(n-11)+7864320*a(n-12),
 *            a(1)=5, a(2)=57, a(3)=2025, a(4)=103373, a(5)=6262064,
 *            a(6)=404495219, a(7)=26715084257, a(8)=1777603386801,...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0)
        return 0;

    /* Static initialization of first 13 terms */
    static seq_t initvals[13];
    static int init_done = 0;
    if (!init_done) {
        const char *strs[13] = {
            "5",
            "57",
            "2025",
            "103373",
            "6262064",
            "404495219",
            "26715084257",
            "1777603386801",
            "118573281463225",
            "7915782848604211",
            "528589085007311295",
            "35300530985616790273",
            "2357529150039721810029"
        };
        for (idx_t i = 0; i < 13; ++i) {
            initvals[i] = seq_from_str(strs[i]);
        }
        init_done = 1;
    }

    if (n <= 13)
        return initvals[n - 1];

    /* Coefficients for the linear recurrence (a(n-1) ... a(n-12)) */
    static const long long coef[12] = {
        100LL,
        -2365LL,
        6020LL,
        291532LL,
        -2681320LL,
        4701820LL,
        23602304LL,
        -82586672LL,
        27026368LL,
        113762816LL,
        -78159872LL,
        7864320LL
    };

    /* Recent 12 values: recent[0] = a(cur-1), recent[1] = a(cur-2), ... */
    seq_t recent[12];
    /* Initialize with a(13), a(12), ..., a(2) */
    for (int k = 0; k < 12; ++k) {
        recent[k] = initvals[12 - k];
    }

    for (idx_t cur = 14; cur <= n; ++cur) {
        seq_t next = 0;
        for (int k = 0; k < 12; ++k) {
            long long c = coef[k];
            if (c >= 0)
                next += recent[k] * (seq_t)c;
            else
                next -= recent[k] * (seq_t)(-c);
        }
        /* Shift recent values */
        for (int k = 11; k > 0; --k)
            recent[k] = recent[k - 1];
        recent[0] = next;
    }

    return recent[0];
}
