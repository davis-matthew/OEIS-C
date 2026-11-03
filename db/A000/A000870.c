/*
 * A000870  Numbers beginning with letter 's' in English.
 * https://oeis.org/A000870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   numbers beginning with letter 's' in English when spelled out
 * Range:     u8 n <= 24, u16 n <= 9760, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 7402 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int starts_with_s_group(seq_t group) {
    if (group == 6 || group == 7) return 1;
    if (group == 16 || group == 17) return 1;
    if (group >= 60 && group < 80) return 1;
    if (group >= 600 && group < 800) return 1;
    return 0;
}

static int starts_with_s(seq_t num) {
    while (num >= 1000) {
        num /= 1000;
    }
    return starts_with_s_group(num);
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    while (count < n) {
        num++;
        if (starts_with_s(num)) {
            count++;
        }
    }
    return num;
}
